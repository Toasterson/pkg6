//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <gtest/gtest.h>
#include <package/PackageInfo.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <catalog/handler/storage/JSONPackageSerializer.h>
#include "rapidjson/stringbuffer.h"

using namespace pkg;

namespace {
    class PKGINFOTest : public ::testing::Test {
    public:
        PKGINFOTest():
                pkg1("pkg://openindiana.org/database/postgres/pgadmin@1.18.1,5.11-2016.0.0.0:20160730T011750Z"),
                //TODO Fix add proper json here
                pgadmin_string("{\"publisher\":\"openindiana.org\",\"name\":\"database/postgres/pgadmin\",\"version\":\"1.18.1\",\"build\":\"5.11\",\"branch\":\"2016.0.0.0\",\"packaging_date\":\"20160730T011750Z\",\"summary\":\"pgAdminIII administration and development GUI for PostgreSQL\",\"classifications\":[\"org.opensolaris.category.2008:System/Databases\"],\"states\":[0,7],\"attrs\":[{\"variant.arch\":[\"i386\"]},{\"userland.info.git-remote\":[\"git://github.com/OpenIndiana/oi-userland.git\"]},{\"com.oracle.info.name\":[\"pgadmin3\"]},{\"userland.info.git-branch\":[\"HEAD\"]},{\"userland.info.git-rev\":[\"be2e50cbe85afb57a066785d065c91b1a7348842\"]},{\"info.source-url\":[\"http://ftp.postgresql.org/pub/pgadmin3/release/v1.18.1/src/pgadmin3-1.18.1.tar.gz\"]},{\"info.upstream-url\":[\"http://www.pgadmin.org\"]},{\"org.opensolaris.consolidation\":[\"userland\"]},{\"com.oracle.info.version\":[\"1.18.1\"]},{\"pkg.debug.depend.bypassed\":[\"usr/lib/libpq.so.5\"]}],\"dependencies\":[{\"fmri\":\"database/postgres/pgagent\",\"type\":\"require\"},{\"fmri\":\"pkg:/database/postgres-93/library@9.3.13-2016.0.0.1\",\"type\":\"require\"},{\"fmri\":\"pkg:/library/graphics/wxwidgets@2.8.12-2016.0.0.0\",\"type\":\"require\"},{\"fmri\":\"pkg:/library/libxml2@2.9.4-2016.0.0.0\",\"type\":\"require\"},{\"fmri\":\"pkg:/library/libxslt@1.1.28-2016.0.0.2\",\"type\":\"require\"},{\"fmri\":\"pkg:/library/security/openssl@1.0.2.8-2016.0.0.3\",\"type\":\"require\"},{\"fmri\":\"pkg:/library/zlib@1.2.8-2016.0.0.0\",\"type\":\"require\"},{\"fmri\":\"pkg:/system/library/g++-4-runtime@4.9.3-2016.0.0.2\",\"type\":\"require\"},{\"fmri\":\"pkg:/system/library/gcc-4-runtime@4.9.3-2016.0.0.2\",\"type\":\"require\"},{\"fmri\":\"pkg:/system/library/math@0.5.11-2016.0.0.15685\",\"type\":\"require\"},{\"fmri\":\"pkg:/system/library@0.5.11-2016.0.0.15685\",\"type\":\"require\"}]}")
        {}
        virtual void SetUp(){
            Document doc;
            doc.Parse(pgadmin_string.c_str());
            pgadmin = ser.Deserialize(doc);
        }
        JSONPackageSerializer ser;
        std::string pgadmin_string;
        pkg::PackageInfo pkg1;
        pkg::PackageInfo pgadmin;
    };

    TEST_F(PKGINFOTest, CreationProperties){
        ASSERT_STREQ("openindiana.org/database/postgres/pgadmin@1.18.1,5.11-2016.0.0.0:20160730T011750Z", pkg1.getFmri().c_str());
    }

    TEST_F(PKGINFOTest, AddSummary){
        pkg1.addAction("set name=pkg.summary value=Testing");
        ASSERT_STREQ("Testing", pkg1.summary.c_str());
        ASSERT_EQ(0, pkg1.attrs.size());
    }

    TEST_F(PKGINFOTest, Obsolete){
        pkg1.addAction("set name=pkg.summary value=Testing");
        pkg1.addAction("set name=pkg.obsolete value=true");
        ASSERT_EQ(pkg1.obsolete, true);
        ASSERT_EQ(pkg1.summary.empty(), true);
    }

    TEST_F(PKGINFOTest, RandomMetaData){
        pkg1.addAction("set name=pkg.testing value=Testing value=Testing2 optional=Test");
        ASSERT_STREQ("pkg.testing", pkg1.attrs[0].name.c_str());
        ASSERT_STREQ("Testing", pkg1.attrs[0].values[0].c_str());
        ASSERT_STREQ("Testing2", pkg1.attrs[0].values[1].c_str());
        ASSERT_STREQ("Test", pkg1.attrs[0].optionals["optional"].c_str());
    }

    TEST_F(PKGINFOTest, AddDependency){
        pkg1.addAction("depend fmri=system/library type=require");
        ASSERT_EQ(1, pkg1.dependencies.size());
    }

    TEST_F(PKGINFOTest, MergeTest){
        pkg1.addAction("set name=Testing value=Test");
        int origsize = (int) pgadmin.attrs.size();
        pkg1 += pgadmin;
        ASSERT_STREQ("pgAdminIII administration and development GUI for PostgreSQL", pkg1.summary.c_str());
        ASSERT_EQ(origsize+1, pkg1.attrs.size());
        ASSERT_STREQ("Testing", pkg1.attrs[0].name.c_str());
    }

    TEST_F(PKGINFOTest, InOut){
        //TODO Out-In is probably more relaible
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        Document doc;
        ser.Serialize(pgadmin, doc);
        doc.Accept(writer);
        ASSERT_STREQ(pgadmin_string.c_str(), buffer.GetString());
    }
}