// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "chain.h"
#include "chainparams.h"
#include "primitives/block.h"
#include "uint256.h"
#include "util.h"

int GetForkBlock(int nForkBlock)
{
    switch(nForkBlock)
    {
        case 2: // Fork2Block
            if(Params().AllowMinDifficultyBlocks())
                return 36;
            return 21346;
        case 3: // Fork3Block
            return 27260;
        case 4: // Fork4Block
            return 46331;
        case 5: // Fork5Block
            return 999999;
        default:
            if(Params().AllowMinDifficultyBlocks())
                return -1;
            return 20290 - 1;
    }
}

unsigned int GetNextWorkRequired_PID(const CBlockIndex* pindexLast, const CBlockHeader *pblock)
{
    unsigned int i;

    int64_t nTargetTimespanLocal = 0;
    int64_t nIntervalLocal = 0;

    // moved variable inits to the top where they belong

    unsigned int nProofOfWorkLimit = Params().ProofOfWorkLimit().GetCompact();
    int64_t nActualTimespan;
    int64_t lowLimit;
    int64_t highLimit;
    unsigned int blockstogoback = nIntervalLocal; // was -1
    uint256 bnNew;
    const CBlockIndex* pindexFirst = pindexLast;

    int64_t error;
    //int64 diffcalc;
    double pGainUp=-0.005125;	// Theses values can be changed to tune the PID formula
    double iGainUp=-0.0225;	// Theses values can be changed to tune the PID formula
    double dGainUp=-0.0075; 	// Theses values can be changed to tune the PID formula

    double pGainDn=-0.005125;	// Theses values can be changed to tune the PID formula
    double iGainDn=-0.0525;	// Theses values can be changed to tune the PID formula
    double dGainDn=-0.0075; 	// Theses values can be changed to tune the PID formula

    double pCalc;
    double iCalc;
    double dCalc;
    double dResult;
    int64_t result;
    uint256 bResult;
    //uint256 bLowLimit; // Limit for PID calc to never go below this

    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit;

    // Starting from block 20,290 the network diff was set to 16
    // and the retarget interval was changed to 36
    if(pindexLast->nHeight < GetForkBlock(0) && !Params().AllowMinDifficultyBlocks())
    {
        nTargetTimespanLocal = Params().TargetTimespanOld();
        nIntervalLocal = Params().IntervalOld();
    }
    else if(pindexLast->nHeight == GetForkBlock(0) && !Params().AllowMinDifficultyBlocks())
    {
        bnNew.SetCompact(0x1c0ffff0); // Difficulty 16
        LogPrintf("GetNextWorkRequired RETARGET (fork at block 20290)\n");
        LogPrintf("Before: %08x  %s\n", pindexLast->nBits, uint256().SetCompact(pindexLast->nBits).ToString());
        LogPrintf("After:  %08x  %s\n", bnNew.GetCompact(), bnNew.ToString());
        return bnNew.GetCompact();
    }
    else // Keep in for a resync
    {
        nTargetTimespanLocal = Params().TargetTimespan();
        nIntervalLocal = Params().Interval();
    }

    // after fork2Block we retarget every block
    if(pindexLast->nHeight < GetForkBlock(2) && !Params().AllowMinDifficultyBlocks())
    {
        // Only change once per interval
        if ((pindexLast->nHeight+1) % nIntervalLocal != 0 && !Params().AllowMinDifficultyBlocks())
        {
            return pindexLast->nBits;
        }
    }

    // let it walk through 2nd fork stuff if below fork3Block, and ignore if on testnet
    if(pindexLast->nHeight < GetForkBlock(3) && !Params().AllowMinDifficultyBlocks())
    {
        // Catcoin: This fixes an issue where a 51% attack can change difficulty at will.
        // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
        if ((pindexLast->nHeight+1) != nIntervalLocal)
            blockstogoback = nIntervalLocal;

        // Go back by what we want to be 14 days worth of blocks
        printf("Blocks to go back: %d\n", blockstogoback);
        if(blockstogoback > 0)
        {
            for (i = 0; pindexFirst && i < blockstogoback; i++)
                pindexFirst = pindexFirst->pprev;
            assert(pindexFirst);
        }

        // Limit adjustment step
        int numerator = 4;
        int denominator = 1;
        if(pindexLast->nHeight >= GetForkBlock(2)){
            numerator = 112;
            denominator = 100;
        }
        nActualTimespan = pindexLast->GetBlockTime() - pindexFirst->GetBlockTime();
        lowLimit = nTargetTimespanLocal*denominator/numerator;
        highLimit = nTargetTimespanLocal*numerator/denominator;
        LogPrintf("  nActualTimespan = %d  before bounds\n", nActualTimespan);
        if (nActualTimespan < lowLimit)
            nActualTimespan = lowLimit;
        if (nActualTimespan > highLimit)
            nActualTimespan = highLimit;

        // Retarget
        bnNew.SetCompact(pindexLast->nBits);
        bnNew *= nActualTimespan;
        bnNew /= nTargetTimespanLocal;

        if (bnNew > Params().ProofOfWorkLimit())
            bnNew = Params().ProofOfWorkLimit();

        /// debug print
        LogPrintf("GetNextWorkRequired RETARGET\n");
        LogPrintf("Params().TargetTimespan() = %d    nActualTimespan = %d\n", nTargetTimespanLocal, nActualTimespan);
        LogPrintf("Before: %08x  %s\n", pindexLast->nBits, uint256().SetCompact(pindexLast->nBits).ToString());
        LogPrintf("After:  %08x  %s\n", bnNew.GetCompact(), bnNew.ToString());
    }
/*
PID formula
Error = Actual Time - Desired time
P Calc = pGain * Error
I Calc = iGain * Error * (Desired Time / Actual Time)
D Calc = dGain * (Error / Actual Time) * I Calc

New Diff = (Current Diff + P Calc + I Calc + D Calc)

If New diff < 0, then set static value of 0.0001 or so.
*/

    if(pindexLast->nHeight >= GetForkBlock(3) || Params().AllowMinDifficultyBlocks())
        // Fork 3 to use a PID routine instead of the other 2 forks
    {
        pindexFirst = pindexLast->pprev; 	// Set previous block
        for(i=0;i<7;i++) pindexFirst = pindexFirst->pprev; // Set 4th previous block for 8 block filtering
        nActualTimespan = pindexLast->GetBlockTime() - pindexFirst->GetBlockTime(); 	// Get last X blocks time
        nActualTimespan = nActualTimespan / 8; 	// Calculate average for last 8 blocks
//		if(pindexLast->nHeight > fork4Block || fTestNet){
//			if (nMinSpacing > nActualTimespan){
//				printf("WARNING: SANITY CHECK FAILED: PID nActualTimespan %"PRI64d" too small! increased to %"PRI64d"\n",
//					nActualTimespan, nMinSpacing );
//				nActualTimespan = nMinSpacing;
//			}
//		}
        bnNew.SetCompact(pindexLast->nBits);	// Get current difficulty
        i=0;					// Zero bit-shift counter
        while(bnNew>0)				// Loop while bnNew > 0
        {
            i++;				// Increment bit-shift counter
            bnNew = bnNew >> 1;		// shift bnNew lower by 1 bit
            if(i>256) bnNew = 0;		// overflow test, just to make sure that it never stays in this loop
        }
        bnNew.SetCompact(pindexLast->nBits);	// Get current difficulty again


        error = nActualTimespan - Params().TargetSpacing();	// Calculate the error to be fed into the PID Calculation
        if(error >= -450 && error <= 450) // Slower gains for when the average time is within 2.5 min and 7.5 min
        {
            // Calculate P ... pGainUp defined at beginning of routine
            pCalc = pGainUp * (double)error;
            // Calculate I ... iGainUp defined at beginning of routine
            iCalc = iGainUp * (double)error * (double)((double)Params().TargetSpacing() / (double)nActualTimespan);
            // Calculate D ... dGainUp defined at beginning of routine
            dCalc = dGainUp * ((double)error / (double)nActualTimespan) * iCalc;
        }
        else // Faster gains for block averages faster than 2.5 min and greater than 7.5 min
        {
            // Calculate P ... pGainDn defined at beginning of routine
            pCalc = pGainDn * (double)error;
            // Calculate I ... iGainDn defined at beginning of routine
            iCalc = iGainDn * (double)error * (double)((double)Params().TargetSpacing() / (double)nActualTimespan);
            // Calculate D ... dGainDn defined at beginning of routine
            dCalc = dGainDn * ((double)error / (double)nActualTimespan) * iCalc;
        }

        if(error > -10 && error < 10)
        {
            if(Params().AllowMinDifficultyBlocks())
                LogPrintf("Within dead zone. No change!  error: %d\n", error);
            return(bnNew.GetCompact());
        }

        dResult = pCalc + iCalc + dCalc;	// Sum the PID calculations

        result = (int64_t)(dResult * 65536);	// Adjust for scrypt calcuation
        // Bring the result within max range to avoid overflow condition
        while(result >  8388607) result = result / 2;
        bResult = result;			// Set the bignum value
        if(i>24) bResult = bResult << (i - 24);	// bit-shift integer value of result to be subtracted from current diff

        LogPrintf("pCalc: %f, iCalc: %f, dCalc: %f, Result: %d (%f)\n", pCalc, iCalc, dCalc, result, dResult);
        LogPrintf("Actual Time: %d, error: %d\n", nActualTimespan, error);
        LogPrintf("Result: %08x %s\n",bResult.GetCompact(), bResult.ToString());
        LogPrintf("Before: %08x %s\n",bnNew.GetCompact(), bnNew.ToString());
        bnNew = bnNew - bResult; 	// Subtract the result to set the current diff

        // Make sure that diff is not set too low, ever
        if (bnNew.GetCompact() > 0x1e0fffff) bnNew.SetCompact(0x1e0fffff);
        if(Params().AllowMinDifficultyBlocks())
            LogPrintf("After:  %08x %s\n",bnNew.GetCompact(), bnNew.ToString());

    } // End Fork 3 to use a PID routine instead of the other 2 forks routine

    return bnNew.GetCompact();
}

unsigned int GetNextWorkRequired_DIGI(const CBlockIndex* pindexLast, const CBlockHeader *pblock)
{
    // Digi algorithm should never be used until at least 2 blocks are mined.
    // Contains code by RealSolid & WDC
    // Cleaned up for use in Guldencoin by GeertJohan (dead code removal since Guldencoin retargets every block)
    // retarget timespan is set to a single block spacing because there is a retarget every block
    int64_t retargetTimespan = Params().TargetSpacing();

    // get previous block
    const CBlockIndex* pindexPrev = pindexLast->pprev;
    assert(pindexPrev);

    // calculate actual timestpan between last block and previous block
    int64_t nActualTimespan = pindexLast->GetBlockTime() - pindexPrev->GetBlockTime();
    LogPrintf("Digishield retarget\n");
    LogPrintf("nActualTimespan = %d before bounds\n", nActualTimespan);

    // limit difficulty changes between 50% and 125% (human view)
    if (nActualTimespan < (retargetTimespan - (retargetTimespan/4)) ) nActualTimespan = (retargetTimespan - (retargetTimespan/4));
    if (nActualTimespan > (retargetTimespan + (retargetTimespan/2)) ) nActualTimespan = (retargetTimespan + (retargetTimespan/2));

    // calculate new difficulty
    uint256 bnNew;
    bnNew.SetCompact(pindexLast->nBits);
    bnNew *= nActualTimespan;
    bnNew /= retargetTimespan;

    // difficulty should never go below (human view) the starting difficulty
    if (bnNew > Params().ProofOfWorkLimit())
        bnNew = Params().ProofOfWorkLimit();

    /// debug print
    LogPrintf("nTargetTimespan = %d nActualTimespan = %d\n" , retargetTimespan, nActualTimespan);
    LogPrintf("Before: %08x %s\n", pindexLast->nBits, uint256().SetCompact(pindexLast->nBits).ToString());
    LogPrintf("After: %08x %s\n", bnNew.GetCompact(), bnNew.ToString());

    // Make sure that diff is not set too low, ever ... Might find a min diff higher than 0.000228882 (or 15 on cgminer's output)
    if (bnNew.GetCompact() > 0x1e0fffff) bnNew.SetCompact(0x1e0fffff);

    return bnNew.GetCompact();
}

//unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock)
//{
//    unsigned int nProofOfWorkLimit = Params().ProofOfWorkLimit().GetCompact();
//
//    // Genesis block
//    if (pindexLast == NULL)
//        return nProofOfWorkLimit;
//
//    // Only change once per interval
//    if ((pindexLast->nHeight+1) % Params().Interval() != 0)
//    {
//        if (Params().AllowMinDifficultyBlocks())
//        {
//            // Special difficulty rule for testnet:
//            // If the new block's timestamp is more than 2* 10 minutes
//            // then allow mining of a min-difficulty block.
//            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + Params().TargetSpacing()*2)
//                return nProofOfWorkLimit;
//            else
//            {
//                // Return the last non-special-min-difficulty-rules-block
//                const CBlockIndex* pindex = pindexLast;
//                while (pindex->pprev && pindex->nHeight % Params().Interval() != 0 && pindex->nBits == nProofOfWorkLimit)
//                    pindex = pindex->pprev;
//                return pindex->nBits;
//            }
//        }
//        return pindexLast->nBits;
//    }
//
//    // Catcoin: This fixes an issue where a 51% attack can change difficulty at will.
//    // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
//    int blockstogoback = Params().Interval()-1;
//    if ((pindexLast->nHeight+1) != Params().Interval())
//        blockstogoback = Params().Interval();
//
//    // Go back by what we want to be 14 days worth of blocks
//    const CBlockIndex* pindexFirst = pindexLast;
//    for (int i = 0; pindexFirst && i < blockstogoback; i++)
//        pindexFirst = pindexFirst->pprev;
//    assert(pindexFirst);
//
//    // Limit adjustment step
//    int64_t nActualTimespan = pindexLast->GetBlockTime() - pindexFirst->GetBlockTime();
//    LogPrintf("  nActualTimespan = %d  before bounds\n", nActualTimespan);
//    if (nActualTimespan < Params().TargetTimespan()/4)
//        nActualTimespan = Params().TargetTimespan()/4;
//    if (nActualTimespan > Params().TargetTimespan()*4)
//        nActualTimespan = Params().TargetTimespan()*4;
//
//    // Retarget
//    uint256 bnNew;
//    uint256 bnOld;
//    bnNew.SetCompact(pindexLast->nBits);
//    bnOld = bnNew;
//    // Catcoin: intermediate uint256 can overflow by 1 bit
//    bool fShift = bnNew.bits() > 235;
//    if (fShift)
//        bnNew >>= 1;
//    bnNew *= nActualTimespan;
//    bnNew /= Params().TargetTimespan();
//    if (fShift)
//        bnNew <<= 1;
//
//    if (bnNew > Params().ProofOfWorkLimit())
//        bnNew = Params().ProofOfWorkLimit();
//
//    /// debug print
//    LogPrintf("GetNextWorkRequired RETARGET\n");
//    LogPrintf("Params().TargetTimespan() = %d    nActualTimespan = %d\n", Params().TargetTimespan(), nActualTimespan);
//    LogPrintf("Before: %08x  %s\n", pindexLast->nBits, bnOld.ToString());
//    LogPrintf("After:  %08x  %s\n", bnNew.GetCompact(), bnNew.ToString());
//
//    return bnNew.GetCompact();
//}

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock)
{
    int DiffMode=1;
    //double algoTest=0;
    int64_t timestamp = (pindexLast->GetBlockTime() % 60); // Get the seconds portion of the last block
    uint256 bnNew;
    //algoTest = (pindexLast->nHeight % 2);
    if(pindexLast->nHeight >= GetForkBlock(4) || Params().AllowMinDifficultyBlocks())
    {
        LogPrintf("Timestamp seconds: %d\n", timestamp);
        // Seconds timestamp of the last block between 0 and 14 OR between 30 and 44, then use Diff mode 0 (Digi), else use Diff mode 1 (PID)
        if((timestamp >=0 && timestamp <= 14) ||
           (timestamp >=30 && timestamp <= 44))
        {
            DiffMode = 0;
        }
        //if(algoTest == 0) { DiffMode = 0; }
    }
    else
    {
        DiffMode = 1; // Old algo
    }

// ******************** Uncomment to reset for testnet
//	if(Params().AllowMinDifficultyBlocks())
//	{
//        bnNew.SetCompact(0x1e0fffff); return bnNew.GetCompact();
//    }
// ********************

    if(DiffMode == 0) // DigiShield algo
    {
        return GetNextWorkRequired_DIGI(pindexLast, pblock);
    }
    else // PID Algo
    {
        return GetNextWorkRequired_PID(pindexLast, pblock);
    }

    // It should never get to this next line, but force PID algo, just in case it does.
    return GetNextWorkRequired_PID(pindexLast, pblock);
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits)
{
    bool fNegative;
    bool fOverflow;
    uint256 bnTarget;

    if (Params().SkipProofOfWorkCheck())
       return true;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > Params().ProofOfWorkLimit())
        return error("CheckProofOfWork() : nBits below minimum work");

    // Check proof of work matches claimed amount
    if (hash > bnTarget)
        return error("CheckProofOfWork() : hash doesn't match nBits");

    return true;
}

uint256 GetBlockProof(const CBlockIndex& block)
{
    uint256 bnTarget;
    bool fNegative;
    bool fOverflow;
    bnTarget.SetCompact(block.nBits, &fNegative, &fOverflow);
    if (fNegative || fOverflow || bnTarget == 0)
        return 0;
    // We need to compute 2**256 / (bnTarget+1), but we can't represent 2**256
    // as it's too large for a uint256. However, as 2**256 is at least as large
    // as bnTarget+1, it is equal to ((2**256 - bnTarget - 1) / (bnTarget+1)) + 1,
    // or ~bnTarget / (nTarget+1) + 1.
    return (~bnTarget / (bnTarget + 1)) + 1;
}
