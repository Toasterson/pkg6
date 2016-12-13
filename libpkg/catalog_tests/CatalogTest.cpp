//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "mock_ICatalogIO.h"
#include <catalog/Catalog.h>

using ::testing::Return;

namespace {

    TEST(CatalogTest, importLegacy){
        MockICatalogIO mockICatalogIO("");
        EXPECT_CALL(mockICatalogIO, createStatePath()).WillRepeatedly(Return(true));
    }

    TEST(CatalogTest, Create){

    }

    TEST(CatalogTest, ResolvePackage){

    }

    TEST(CatalogTest, addPackage){

    }

    TEST(CatalogTest, UpdatePackage){

    }

    TEST(CatalogTest, removePackage){

    }

    TEST(CatalogTest, savePackage){

    }

    TEST(CatalogTest, loadPackage){

    }

    TEST(CatalogTest, containsPackage){

    }

}