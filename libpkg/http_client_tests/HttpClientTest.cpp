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
                client(HttpClient("http", "pkg.openindiana.org", "/hipster", "openindiana.org"))
        {}
        virtual void SetUp(){

        }

        HttpClient client;

    };

    TEST_F(HTTPTest, Versions0){
        std::map<std::string,int> ver = client.getVersion();
        ASSERT_EQ(1, ver["catalog"]);
        ASSERT_EQ(0, ver["p5i"]);
        ASSERT_EQ(1, ver["publisher"]);
    }

    TEST_F(HTTPTest, Manifest0){
        std::stringstream ss;
        client.getManifest("web/server/nginx@1.11.3,5.11-2016.0.0.0:20160730T022405Z", ss);
        ASSERT_GT(ss.str().size(), 1);
    }

    TEST_F(HTTPTest, File1){
        std::stringstream ss;
        client.getFile("e85098cd7a821179b8620b48dc12d04099bff3a3", ss);
        ASSERT_GT(ss.str().size(), 1);
    }
}