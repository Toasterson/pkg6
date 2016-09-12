//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <action/FileAction.h>
#include <gtest/gtest.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace pkg::action;

namespace {
    class FileActionTest: public ::testing::Test {
    public:
        FileActionTest():
                file_string("file 84ecd47ef66bb9be2f7cce732e4ec67af6c473ff path=usr/share/caja/browser.xml pkg.size=3988 pkg.csize=1145 chash=1eb0bf5396f154ae9646be477a2ad990204f4da2 group=bin mode=0444 owner=root pkg.depend.bypass-generate=usr/lib(.*)/libpq.so.*"),
                action(file_string)
                {}

        virtual void SetUp() {}

        string file_string;
        FileAction action;
    };


    TEST_F(FileActionTest, InOut){
        //Serialize Action into string
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        action.Serialize(writer);
        //Deserialize Action into new Object
        Document doc;
        doc.Parse(buffer.GetString());
        FileAction action2;
        action2.Deserialize(doc);
        //Compare name size csize owner path group mode chash
        ASSERT_STREQ(action.name.c_str(), action2.name.c_str());
        ASSERT_EQ(action.size, action2.size);
        ASSERT_EQ(action.csize, action2.csize);
        ASSERT_STREQ(action.attributes["path"].c_str(), action2.attributes["path"].c_str());
        ASSERT_STREQ(action.attributes["owner"].c_str(), action2.attributes["owner"].c_str());
        ASSERT_STREQ(action.attributes["group"].c_str(), action2.attributes["group"].c_str());
        ASSERT_STREQ(action.attributes["mode"].c_str(), action2.attributes["mode"].c_str());
        ASSERT_STREQ(action.attributes["chash"].c_str(), action2.attributes["chash"].c_str());
    }

    TEST_F(FileActionTest, Create){
        ASSERT_STREQ("84ecd47ef66bb9be2f7cce732e4ec67af6c473ff", action.name.c_str());
        ASSERT_STREQ("bin", action.attributes["group"].c_str());
        ASSERT_STREQ("usr/share/caja/browser.xml", action.path.c_str());
        ASSERT_EQ(3988, action.size);
        ASSERT_EQ(1145, action.csize);
    }

    TEST_F(FileActionTest, Stringify){
        ASSERT_STREQ(file_string.c_str(), action.toActionString().c_str());
    }
}