//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_IMAGECONFIG_H
#define PKG6_IMAGECONFIG_H

#include <string>
#include <map>
#include <vector>
#include <boost/filesystem/operations.hpp>
#include <publisher/Publisher.h>


namespace pkg {
    class ImageConfig {
    private:
        const std::string IMAGE_CONFIG_OLDFILENAME = "pkg5.image";
        const std::string IMAGE_CONFIG_FILENAME = "pkg6.image";
        int VERSION = 4;
        string IMAGE_ROOT;
        map<string, Publisher> publishers;
        map<string, string> properties;
        map<string, string> variants;
    public:
        int getVersion() { return VERSION; };


        std::string getVariant(const std::string& name);
        pkg::Publisher getPublisher(const std::string& name);
        pkg::Publisher getFirstPublisher();
        std::string getImageProperty(const std::string& name);

        void set(const std::string& path, const std::string& value);

        ImageConfig()= default;
        ImageConfig(const std::string& root);
        void load(const std::istream& config);
        void save();
        void save(std::ostream& output);
        void upgrade_format(const std::string &newRoot);
        void importpkg5(std::istream& oldconfig);
    };
};


#endif //PKG6_IMAGECONFIG_H
