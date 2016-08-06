//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_IMAGECONFIG_H
#define PKG6_IMAGECONFIG_H

#include <string>
#include <map>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem/operations.hpp>

namespace pkg {
    class ImageConfig {
    private:
        const std::string IMAGE_CONFIG_OLDFILENAME = "pkg5.image";
        const std::string IMAGE_CONFIG_FILENAME = "pkg6.image";
        std::string root;
        boost::property_tree::ptree pt;
        void importpkg5();
    public:
        std::string getVariant(std::string name);
        std::string getFacet(std::string name);
        std::string getPublisher(std::string name);
        std::string getMediator(std::string name);
        std::string getImageProperty(std::string name);

        std::vector<std::string> getSectionVariant(std::vector<std::string>& filter);
        std::vector<std::string> getSectionFacet(std::vector<std::string>& filter);
        std::vector<std::string> getSectionPublisher(std::vector<std::string>& filter);
        std::vector<std::string> getSectionMediator(std::vector<std::string>& filter);
        std::vector<std::string> getSectionImage(std::vector<std::string>& filter);

        ImageConfig()= default;
        ImageConfig(const std::string &root);
        void load();
        void save();
        void upgrade_format(const std::string &newRoot = "");
    };
};


#endif //PKG6_IMAGECONFIG_H
