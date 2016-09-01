//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <http_client/HttpClient.h>
#include <gtest/gtest.h>

namespace {
    class HTTPTest: public ::testing::Test{
    public:
        HTTPTest():
                client(HttpClient("", "openindiana.org"))
                {}
        virtual void SetUp(){

        }

        HttpClient client;

    };

    TEST_F(HTTPTest, Versions0){

    }
}