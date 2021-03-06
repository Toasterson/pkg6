//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <gtest/gtest.h>
#include <image/ImageConfig.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include "rapidjson/stringbuffer.h"

const char *pkg5conf =
            "[authority_openindiana.org]" "\n"
            "ssl_key=" "\n"
            "repo.refresh_seconds=" "\n"
            "mirrors=[]" "\n"
            "sticky=True" "\n"
            "disabled=False" "\n"
            "intermediate_certs=[]" "\n"
            "prefix=openindiana.org" "\n"
            "repo.registration_uri=" "\n"
            "uuid=d9b5f5c4-597e-11e6-a419-88002036722f" "\n"
            "revoked_ca_certs=[]" "\n"
            "repo.related_uris=[]" "\n"
            "repo.description=" "\n"
            "signing_ca_certs=[]" "\n"
            "repo.legal_uris=[]" "\n"
            "origin_info=[{'uri': 'http://pkg.openindiana.org/hipster/', 'proxy': ''}]" "\n"
            "repo.registered=False" "\n"
            "approved_ca_certs=[]" "\n"
            "ssl_cert=" "\n"
            "origins=['http://pkg.openindiana.org/hipster/']" "\n"
            "repo.name=Test" "\n"
            "repo.sort_policy=priority" "\n"
            "alias=" "\n"
            "mirror_info=[]" "\n"
            "repo.collection_type=core" "\n"
            "[image]" "\n"
            "version=4" "\n"
            "[variant]" "\n"
            "variant.opensolaris.zone=global" "\n"
            "variant.arch=i386" "\n"
            "[property]" "\n"
            "send-uuid=True" "\n"
            "mirror-discovery=False" "\n"
            "use-system-repo=False" "\n"
            "publisher-search-order=['openindiana.org']" "\n"
            "flush-content-cache-on-success=True" "\n"
            "dehydrated=[]" "\n"
            "check-certificate-revocation=False" "\n"
            "content-update-policy=default" "\n"
            "be-policy=default" "\n"
            "preferred-authority=" "\n"
            "signature-required-names=[]" "\n"
            "ca-path=/etc/openssl/certs" "\n"
            "[CONFIGURATION]" "\n"
            "version=3" "\n";

namespace {
    class ImageConfigTest : public ::testing::Test {
    public:
        ImageConfigTest(){}

        virtual void SetUp() {
            stringstream ss(pkg5conf);
            conf.importpkg5(ss);
        }
        pkg::ImageConfig conf;
    };

    TEST_F(ImageConfigTest, importpkg5){
        ASSERT_STREQ("d9b5f5c4-597e-11e6-a419-88002036722f", conf.getPublisher("openindiana.org").getGeneralProperty("uuid").c_str());
        ASSERT_STREQ("http://pkg.openindiana.org/hipster/", conf.getPublisher("openindiana.org").getOrigins()[0].c_str());
    }

    TEST_F(ImageConfigTest, saveandload){
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        conf.Serialize(writer);
        ASSERT_GT(buffer.GetSize(), 4);
        Document doc;
        doc.Parse(buffer.GetString());
        pkg::ImageConfig conf2;
        conf2.Deserialize(doc);
        ASSERT_STREQ("False", conf2.getImageProperty("use-system-repo").c_str());
        ASSERT_STREQ("default", conf2.getImageProperty("be-policy").c_str());
        ASSERT_STREQ("i386", conf2.getVariant("arch").c_str());
        ASSERT_STREQ("Test", conf2.getPublisher("openindiana.org").getRepoProperty("name").c_str());
        ASSERT_STREQ("d9b5f5c4-597e-11e6-a419-88002036722f", conf2.getPublisher("openindiana.org").getGeneralProperty("uuid").c_str());
        ASSERT_STREQ("http://pkg.openindiana.org/hipster/", conf2.getPublisher("openindiana.org").getOrigins()[0].c_str());
    }

    TEST_F(ImageConfigTest, Variant){
        ASSERT_STREQ("global", conf.getVariant("opensolaris.zone").c_str());
        ASSERT_STREQ("i386", conf.getVariant("arch").c_str());
    }

    TEST_F(ImageConfigTest, set){
        conf.setImageProperty("Testing", "Test");
        ASSERT_STREQ("Test", conf.getImageProperty("Testing").c_str());
        conf.setVariant("Testing", "Test");
        ASSERT_STREQ("Test", conf.getVariant("Testing").c_str());
        pkg::Publisher pub("http://test.org");
        conf.setPublisher("test.org", pub);
        ASSERT_STREQ("http://test.org", conf.getPublisher("test.org").getOrigins()[0].c_str());
    }

    TEST_F(ImageConfigTest, getOrigins){
        ASSERT_STREQ("http://pkg.openindiana.org/hipster/", conf.getPublisher("openindiana.org").getOrigins()[0].c_str());
    }

    TEST_F(ImageConfigTest, getFirstPublisher){
        pkg::Publisher pub = conf.getFirstPublisher();
        ASSERT_STREQ("openindiana.org", pub.getGeneralProperty("prefix").c_str());
    }
}