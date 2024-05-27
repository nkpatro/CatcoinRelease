// Copyright (c) 2011-2013 The Bitcoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Unit tests for block-chain checkpoints
//

#include "checkpoints.h"

#include "uint256.h"

#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE(Checkpoints_tests)

BOOST_AUTO_TEST_CASE(sanity)
{
    uint256 p5000 = uint256("0xec268a9cfe87adb4d10a147a544493406c80e00a3e6868641e520b184e7ddce3");
    uint256 p30000 = uint256("0xff05303dc58caf2d102c85a0504ed16939c7840c91f5f0b37a5bf128e9afb73f");
    BOOST_CHECK(Checkpoints::CheckBlock(5000, p5000));
    BOOST_CHECK(Checkpoints::CheckBlock(30000, p30000));

    
    // Wrong hashes at checkpoints should fail:
    BOOST_CHECK(!Checkpoints::CheckBlock(5000, p30000));
    BOOST_CHECK(!Checkpoints::CheckBlock(30000, p5000));

    // ... but any hash not at a checkpoint should succeed:
    BOOST_CHECK(Checkpoints::CheckBlock(5000+1, p30000));
    BOOST_CHECK(Checkpoints::CheckBlock(30000+1, p5000));

    BOOST_CHECK(Checkpoints::GetTotalBlocksEstimate() >= 100000);
}    

BOOST_AUTO_TEST_SUITE_END()
