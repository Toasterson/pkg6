//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <gtest/gtest.h>
#include "mock_CatalogStorage.h"
#include <gmock/gmock.h>
#include <catalog/Catalog.h>

using ::testing::Return;
using ::testing::AtLeast;
using ::testing::_;
using namespace pkg;

namespace {

    TEST(CatalogTest, Create){
        MockICatalogStorage mock;
        EXPECT_CALL(mock, create()).Times(AtLeast(1));
        Catalog cat(mock, mock);
        cat.create();
    }

    TEST(CatalogTest, ResolvePackage){
        MockICatalogStorage mock;
        EXPECT_CALL(mock, packageExists(_)).Times(AtLeast(1)).WillRepeatedly(Return(true));
        Catalog cat(mock, mock);
        cat.getPackage("package");
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