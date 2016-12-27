//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <action/DependAction.h>
#include <gtest/gtest.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <catalog/handler/storage/JSONPackageSerializer.h>

using namespace pkg::action;

namespace {
    class DependActionTest: public ::testing::Test{
    public:
        DependActionTest():
                depend_json("{\"fmri\":\"pkg:/system/library@0.5.11-2016.0.0.15685\",\"type\":\"require\"}"),
                act_string("depend fmri=pkg:/system/library@0.5.11-2016.0.0.15685 type=require")
        {}

        virtual void SetUp(){
            Document doc;
            doc.Parse(depend_json.c_str());
            SerializeTest = ser.DeserializeDependAction(doc);
            createTest = DependAction(act_string);
        }
        JSONPackageSerializer ser;
        DependAction SerializeTest;
        DependAction createTest;
        std::string depend_json;
        std::string act_string;
    };

    TEST_F(DependActionTest, Deserialize){
        ASSERT_STREQ("pkg:/system/library@0.5.11-2016.0.0.15685", SerializeTest.fmri.c_str());
        ASSERT_STREQ("require", SerializeTest.type.c_str());
    }

    TEST_F(DependActionTest, Serialize){
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        ser.SerializeDependencyAction(SerializeTest, writer);
        ASSERT_STREQ(depend_json.c_str(), buffer.GetString());
    }

    TEST_F(DependActionTest, Create){
        ASSERT_STREQ("pkg:/system/library@0.5.11-2016.0.0.15685", createTest.fmri.c_str());
        ASSERT_STREQ("require", createTest.type.c_str());
    }

    TEST_F(DependActionTest, Stringify){
        std::string tmp = createTest.toActionString();
        ASSERT_STREQ(act_string.c_str(), tmp.c_str());
    }
}