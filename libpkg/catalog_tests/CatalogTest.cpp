//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <gtest/gtest.h>
#include <catalog/Catalog.h>

namespace {
    class CatalogTest : public ::testing::Test {
    public:
        CatalogTest(): catalog{"/home/toast/workspace/illumos/ips/pkg5/state", "known"} {}

        virtual void SetUp() {

        }
        pkg::Catalog catalog;
    };

    TEST_F(CatalogTest, UpgradeFormat){

    }

    TEST_F(CatalogTest, Create){

    }

    TEST_F(CatalogTest, ResolvePackage){

    }

    TEST_F(CatalogTest, addPackage){

    }

    TEST_F(CatalogTest, UpdatePackage){

    }

    TEST_F(CatalogTest, removePackage){

    }

    TEST_F(CatalogTest, savePackage){

    }

    TEST_F(CatalogTest, loadPackage){

    }

    TEST_F(CatalogTest, containsPackage){

    }

}