// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "uritests.h"

#include "guiutil.h"
#include "walletmodel.h"

#include <QUrl>

void URITests::uriTests()
{
    SendCoinsRecipient rv;
    QUrl uri;
    uri.setUrl(QString("catcoin:9ZFLXoWBYoiLRtzVLWXhcM27Lw2ARhFB9r?req-dontexist="));
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("catcoin:9ZFLXoWBYoiLRtzVLWXhcM27Lw2ARhFB9r?dontexist="));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("9ZFLXoWBYoiLRtzVLWXhcM27Lw2ARhFB9r"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 0);

    uri.setUrl(QString("catcoin:9ZFLXoWBYoiLRtzVLWXhcM27Lw2ARhFB9r?label=Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("9ZFLXoWBYoiLRtzVLWXhcM27Lw2ARhFB9r"));
    QVERIFY(rv.label == QString("Wikipedia Example Address"));
    QVERIFY(rv.amount == 0);

    uri.setUrl(QString("catcoin:9ZFLXoWBYoiLRtzVLWXhcM27Lw2ARhFB9r?amount=0.001"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("9ZFLXoWBYoiLRtzVLWXhcM27Lw2ARhFB9r"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 100000);

    uri.setUrl(QString("catcoin:9ZFLXoWBYoiLRtzVLWXhcM27Lw2ARhFB9r?amount=1.001"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("9ZFLXoWBYoiLRtzVLWXhcM27Lw2ARhFB9r"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 100100000);

    uri.setUrl(QString("catcoin:9ZFLXoWBYoiLRtzVLWXhcM27Lw2ARhFB9r?amount=100&label=Wikipedia Example"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("9ZFLXoWBYoiLRtzVLWXhcM27Lw2ARhFB9r"));
    QVERIFY(rv.amount == 10000000000LL);
    QVERIFY(rv.label == QString("Wikipedia Example"));

    uri.setUrl(QString("catcoin:9ZFLXoWBYoiLRtzVLWXhcM27Lw2ARhFB9r?message=Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("9ZFLXoWBYoiLRtzVLWXhcM27Lw2ARhFB9r"));
    QVERIFY(rv.label == QString());

    QVERIFY(GUIUtil::parseBitcoinURI("catcoin://9ZFLXoWBYoiLRtzVLWXhcM27Lw2ARhFB9r?message=Wikipedia Example Address", &rv));
    QVERIFY(rv.address == QString("9ZFLXoWBYoiLRtzVLWXhcM27Lw2ARhFB9r"));
    QVERIFY(rv.label == QString());

    uri.setUrl(QString("catcoin:9ZFLXoWBYoiLRtzVLWXhcM27Lw2ARhFB9r?req-message=Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("catcoin:9ZFLXoWBYoiLRtzVLWXhcM27Lw2ARhFB9r?amount=1,000&label=Wikipedia Example"));
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("catcoin:9ZFLXoWBYoiLRtzVLWXhcM27Lw2ARhFB9r?amount=1,000.0&label=Wikipedia Example"));
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));
}
