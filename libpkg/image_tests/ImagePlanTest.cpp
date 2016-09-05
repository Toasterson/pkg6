//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <gtest/gtest.h>
#include <image/ImagePlan.h>

namespace {
    class ImagePlanTest : public ::testing::Test {
    public:
        ImagePlanTest():conf(), plan(conf, "./.tmp") {}

        virtual void SetUp() {
            //conf.set("authority_openindiana.org/origins", "http://pkg.openindiana.org/hipster/");
            //plan.add(pkg::PackageInfo("openindiana.org", "web/server/nginx", "1.11.3,5.11-2016.0.0.0:20160730T022405Z"));
        }
        pkg::ImageConfig conf;
        pkg::ImagePlan plan;
    };

    TEST_F(ImagePlanTest, Download){
        //plan.download();
    }

}