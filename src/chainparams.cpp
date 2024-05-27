// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
static Checkpoints::MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (	 4, uint256("0xfe508d41b7dc2c3079e827d4230e6f7ddebca43c9afc721c1e6431f78d6ff1de"))
        (	 5, uint256("0x7fc79021dbfa30255ade9bb8d898640516d9c771c3342a9b889ce380c52c6c1f"))
        ( 5000, uint256("0xec268a9cfe87adb4d10a147a544493406c80e00a3e6868641e520b184e7ddce3"))
        (10000, uint256("0x29c63023c3b8a36b59837734a9c16133a4ef08d9a0e95f639a830c56e415070d"))
        (20000, uint256("0x3a0c072b76a298dabffc4f825a084c0f86dc55fe58f9bf31cc7e21bbfb2ead52"))
        (22500, uint256("0xfd3c87eae2e9be72499978155844598a8675eff7a61c90f9aebcedc94e1b217f"))
        (22544, uint256("0x6dd1a90cc56cf4a46c8c47528c4861c255e86d5f97fcee53ce356174e15c3045"))
        (22554, uint256("0xb13e8b128989f9a9fc1a4c1e547330d0b34d3f60189c00391a116922fa4fcb8c"))
        (22600, uint256("0x9e2d7f2fdab36c3e2b6f0455470cd957c12172ad7877f7c8e414fd736469c8d2"))
        (22650, uint256("0x7afbd354496346819b8a214693af70e1431bfadbf68d49a688ae27539fc6b37e"))
        (22700, uint256("0x35154b803fa5700b69f8081aa6d7c798c1e7fd027971252598a18549092a1291"))
        (22750, uint256("0x67e6eca7d46c1a612b7638e7a503e6dbc7cca4da493f4267833a6f1c9a655a35"))
        (22800, uint256("0x49e84c3b5c261966c37c101ac7691886bd641a382f514c2221735088b1b2beea"))
        (22850, uint256("0xc44cec57381a97c3983df0ef1fcf150669dd1794943202d89b805f423a65516f"))
        (22900, uint256("0x44de4c262de678a23554dd06a6f57270815ea9d145f6c542ab2a8dfbd2ca242c"))
        (22950, uint256("0xcecc4ab30b39fc09bf85eb191e64c1660ab2206c5f80953694997ec5c2db5338"))
        (25890, uint256("0x4806f91100ae83904aa0113cc3acda8fe6ac422186243719a68b76c98e7487c2"))
        (26000, uint256("0x048d01fd25385b61d3c20f99cec5f8c0678d6ad8b5ea3160603184ad11216bfc"))
        (30000, uint256("0xff05303dc58caf2d102c85a0504ed16939c7840c91f5f0b37a5bf128e9afb73f"))
        (35000, uint256("0x8c5b56e660e47b398395fd01fd721b115fe523da400d23c82120c6fd37636423"))
        (40000, uint256("0xb8a6e8aaf4f92d4b521bd022de3008884eba51ff2a5c79e0269d65a03d109283"))
        (41000, uint256("0x88f114a60cb0841735df03cecc3c5662ffbdac184c7344d30cef4f98f5b61ed3"))
        (42000, uint256("0x4a538c3557ab865d74327e38837b5aac63aaebdc4718c2ee7b8101bcdd241eb6"))
        (43000, uint256("0xd2428f19de225b56853090fd548d1d7dd2d3d180b989c785eddb615e60f94209"))
        (44000, uint256("0x587b814e0a113eaf52b94e4920362f4c076d7dc942a4f8c5c4900f2d94adbc26"))
        (44225, uint256("0x324baa0cc239091a60c61af8a332584c02f7c7a11bc07286a4cd6c136310968d"))
        (44227, uint256("0xf8e16629c65f9746deed08631d3e223a9a1d88947c6b41c2107e04b35782726d"))
        (44500, uint256("0x0bc1f9a00a53d9bc88999b2596377bf135ab1f293b886d32a06628abbd2e4d14"))
        (45000, uint256("0x47f0d282919bfd3d9901ac8bf38bb41947517e2c7f80ad6ec8f9a8f09243b7dd"))
        (45500, uint256("0xec9ba2336ab78ad947ccdd2cbe2308a112aabf5379f0cd04cbd224d6e3845714"))
        (46000, uint256("0x2a6410328fdb9800565c49c0eda849e69dc5a414adab2ff73d0aec9dbf0e2458"))
        (46500, uint256("0x96cc990f69f8005d31e09f55455ab28b5a09c70bb89b8c6080b48a3bcc1a9342"))
        (47000, uint256("0x197bd9b1a38262ea8693fd1c0d68e9fdd3cc3cec713d4de0b53fcb4ff70d11fd"))
        (47500, uint256("0xb5618edac7edf58e50409c50802be5e3b99a582f369438d53b7d55e9b334b1b4"))
        (48000, uint256("0xc183240a079a0f05a1ecf9049b6805e9044f9a46d08387208f598600015b8a5e"))
        (48500, uint256("0x0f86070e2a86a4a9480b0adfcc27cfe05dad7e1feb08f98d997b5f9b26351a66"))
        (50000, uint256("0x21803a5005a2637590ecfc401193487bd09b41395c20c31ed27e4f6916295c87"))
        (75000, uint256("0x5607029768611e1e29294cd5987845497c2c681ee2917311ec8cf3b65e61e1c8"))
        (100000,uint256("0x956ca160d703130bce1422ce407eac53064565bfd357fc8ac73e1a7dda9d5764"))
        (116828,uint256("0x750e549a03bcc7f582c7857c162cde7cfbeb6de91cccaf794cbe6337518d6c4c"))
        ;
static const Checkpoints::CCheckpointData data = {
        &mapCheckpoints,
        1502242527, // * UNIX timestamp of last checkpoint block
        245139,   // * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        200.0     // * estimated number of transactions per day after checkpoint
    };

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
        ( 35000, uint256("2af959ab4f12111ce947479bfcef16702485f04afd95210aa90fde7d1e4a64ad"))
        ;
static const Checkpoints::CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
		1369685559,
		37581,
		300
    };

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
        boost::assign::map_list_of
        ( 0, uint256("0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"))
        ;
static const Checkpoints::CCheckpointData dataRegtest = {
        &mapCheckpointsRegtest,
        0,
        0,
        0
    };

class CMainParams : public CChainParams {
public:
    CMainParams() {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /** 
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xfc;
        pchMessageStart[1] = 0xc1;
        pchMessageStart[2] = 0xb7;
        pchMessageStart[3] = 0xdc;
        vAlertPubKey = ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9");
        nDefaultPort = 9933;
        bnProofOfWorkLimit = ~uint256(0) >> 20;
        nSubsidyHalvingInterval = 210000;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 6 * 60 * 60; // 6 hours
        nTargetTimespanOld = 14 * 24 * 60 * 60; // two weeks
        nTargetSpacing = 10 * 60; // 10 minute block time target
        nMaxTipAge = 24 * 60 * 60;

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         * 
         * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
         *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
         *   vMerkleTree: 4a5e1e
         */
        const char* pszTimestamp = "NY Times - December 23, 2013 - For Today's Babes, Toyland Is Digital";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 50 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1387838302;
        genesis.nBits    = 0x1e0ffff0;
        genesis.nNonce   = 588050;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0xbc3b4ec43c4ebb2fef49e6240812549e61ffa623d9418608aa90eaad26c96296"));
        assert(genesis.hashMerkleRoot == uint256("0x4007a33db5d9cdf2aab117335eb8431c8d13fb86e0214031fdaebe69a0f29cf7"));

        vSeeds.push_back(CDNSSeedData("catcointools.com", "dnsseed.catcointools.com"));
        vSeeds.push_back(CDNSSeedData("catcoinpool.org", "dnsseed.catcoinpool.org"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,48);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,176);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0xfd;
        pchMessageStart[1] = 0xcb;
        pchMessageStart[2] = 0xb8;
        pchMessageStart[3] = 0xdd;
        vAlertPubKey = ParseHex("0449623fc74489a947c4b15d579115591add020e53b3490bf47297dfa3762250625f8ecc2fb4fc59f69bdce8f7080f3167808276ed2c79d297054367566038aa82");
        nDefaultPort = 19935;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        nTargetSpacing = 10 * 60; // 10 minute block time target
        nMaxTipAge = 0x7fffffff;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1387838303;
        genesis.nNonce = 608937;
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0xec7987a2ab5225246c5cf9b8d93b4b75bcef383a4a65d5a265bc09ed54006188"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("catcointools.com", "testnet-seed.catcointools.com"));
        vSeeds.push_back(CDNSSeedData("loshan.co.uk", "seed-b.catcoin.loshan.co.uk"));
        vSeeds.push_back(CDNSSeedData("thrasher.io", "dnsseed-testnet.thrasher.io"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        nTargetSpacing = 10 * 60; // 10 minute block time target
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        nMaxTipAge = 24 * 60 * 60;
        genesis.nTime = 1296688602;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 0;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 19944;
        assert(hashGenesisBlock == uint256("0x530827f38f93b43ed12af0b3ad25a288dc02ed74d6d7857862df51fc56c416f9"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return dataRegtest;
    }
};
//static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams {
public:
    CUnitTestParams() {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 18445;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;

        // Catcoin: v2 enforced using Bitcoin's supermajority rule
        nEnforceV2AfterHeight = -1;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval)  { nSubsidyHalvingInterval=anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority)  { nEnforceBlockUpgradeMajority=anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority)  { nRejectBlockOutdatedMajority=anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority)  { nToCheckBlockUpgradeMajority=anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks)  { fDefaultConsistencyChecks=afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) {  fAllowMinDifficultyBlocks=afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams *pCurrentParams = 0;

CModifiableParams *ModifiableParams()
{
   assert(pCurrentParams);
   assert(pCurrentParams==&unitTestParams);
   return (CModifiableParams*)&unitTestParams;
}

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        case CBaseChainParams::UNITTEST:
            return unitTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
