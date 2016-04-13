//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_IMAGECONFIG_H
#define PKG6_IMAGECONFIG_H

#include <string>
#include <map>
#include "../simpleini/SimpleIni.h"

namespace pkg {
    class ImageConfig {
    private:
        const std::string IMAGE_CONFIG_FILENAME = "pkg5.image";
        std::string root;
        //CSimpleIniA ini;
        void upgrade_format();
    public:
        /*std::string getVariant(std::string name);
        std::string getFacet(std::string name);
        std::string getPublisher(std::string name);
        std::string getMediator(std::string name);
        std::string getImageProperty(std::string name);

        std::vector<std::string> getSectionVariant(std::vector<std::string>& filter);
        std::vector<std::string> getSectionFacet(std::vector<std::string>& filter);
        std::vector<std::string> getSectionPublisher(std::vector<std::string>& filter);
        std::vector<std::string> getSectionMediator(std::vector<std::string>& filter);
        std::vector<std::string> getSectionImage(std::vector<std::string>& filter);*/

        ImageConfig()= default;
        ImageConfig(const std::string &root);
        //void load();
        //void save();
    };
};


#endif //PKG6_IMAGECONFIG_H
