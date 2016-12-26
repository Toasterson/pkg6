//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <gtest/gtest.h>
#include "mock_CatalogStorage.h"
#include <gmock/gmock.h>

using ::testing::Return;
using ::testing::AtLeast;

namespace {

    TEST(CatalogTest, Create){
        MockICatalogStorage mock;
        EXPECT_CALL(mock, create()).Times(AtLeast(1));
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