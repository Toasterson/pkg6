//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//
#include <action/AttributeAction.h>
#include <gtest/gtest.h>
#include <rapidjson/document.h>
#include <catalog/handler/storage/JSONPackageSerializer.h>

using namespace pkg::action;

namespace {

    TEST(AttributeAction, Ninth_String){
        const AttributeAction action("set name=info.source-url value=http://www.pgpool.net/download.php?f=pgpool-II-3.3.1.tar.gz");
        EXPECT_STREQ(action.name.c_str(), "info.source-url");
        EXPECT_STREQ(action.values[0].c_str(), "http://www.pgpool.net/download.php?f=pgpool-II-3.3.1.tar.gz");
    }

    TEST(AttributeAction, First_String){
        const AttributeAction action("set name=variant.opensolaris.zone value=global value=nonglobal");
        EXPECT_STREQ(action.name.c_str(), "variant.opensolaris.zone");
        EXPECT_STREQ(action.values[0].c_str(), "global");
        EXPECT_STREQ(action.values[1].c_str(), "nonglobal");
    }

    TEST(AttributeAction, Second_String){
        const AttributeAction action("set name=variant.arch value=i386");
        EXPECT_STREQ(action.name.c_str(), "variant.arch");
        EXPECT_STREQ(action.values[0].c_str(), "i386");
    }

    TEST(AttributeAction, Third_String){
        const AttributeAction action("set name=pkg.summary value=\"Gujarati language support\"");
        EXPECT_STREQ(action.name.c_str(), "pkg.summary");
        EXPECT_STREQ(action.values[0].c_str(), "Gujarati language support");
    }

    TEST(AttributeAction, Fourth_String){
        const AttributeAction action("set name=pkg.summary value=\\\"'XZ Utils - loss-less file compression application and library.'\\\"");
        EXPECT_STREQ(action.name.c_str(), "pkg.summary");
        EXPECT_STREQ(action.values[0].c_str(), "XZ Utils - loss-less file compression application and library.");
    }

    TEST(AttributeAction, Fifth_String){
        const AttributeAction action("set name=pkg.summary value=\\\"provided mouse accessibility enhancements\\\"");
        EXPECT_STREQ(action.name.c_str(), "pkg.summary");
        EXPECT_STREQ(action.values[0].c_str(), "provided mouse accessibility enhancements");
    }

    TEST(AttributeAction, Sixth_String){
        const AttributeAction action("set name=info.upstream value=X.Org Foundation");
        EXPECT_STREQ(action.name.c_str(), "info.upstream");
        EXPECT_STREQ(action.values[0].c_str(), "X.Org Foundation");
    }

    TEST(AttributeAction, Seventh_String){
        const AttributeAction action("set name=pkg.description value=Latvian language support's extra files");
        EXPECT_STREQ(action.name.c_str(), "pkg.description");
        EXPECT_STREQ(action.values[0].c_str(), "Latvian language supports extra files");
    }

    TEST(AttributeAction, Eight_String){
        const AttributeAction action("set name=illumos-gate.info.git-remote value=git://github.com/illumos/illumos-gate.git");
        EXPECT_STREQ(action.name.c_str(), "illumos-gate.info.git-remote");
        EXPECT_STREQ(action.values[0].c_str(), "git://github.com/illumos/illumos-gate.git");
    }

    TEST(AttributeAction, Deserialize){
        Document doc;
        doc.Parse("{\"userland.info.git-remote\":[\"git://github.com/OpenIndiana/oi-userland.git\"]}");
        JSONPackageSerializer ser;
        AttributeAction act = ser.DeserializeAttributeAction(doc);
        EXPECT_STREQ("userland.info.git-remote", act.name.c_str());
        EXPECT_STREQ("git://github.com/OpenIndiana/oi-userland.git", act.values[0].c_str());
    }
}
