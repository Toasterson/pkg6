//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_CATALOGPARTBASE_H
#define PKG6_CATALOGPARTBASE_H

#include <string>
#include <chrono>
#include <map>
#include "PackageInfo.h"
#include "boost/property_tree/ptree.hpp"

namespace pkg {

    class Catalog {

    private:
        bool read_only;
        bool do_sign;
        std::string signature;
        std::string root_dir;
        std::string name;
        std::tm last_modified;

        void importpkg5(const std::string& importDir);
    public:

        Catalog(const std::string &root = "",
                const std::string &name = "",
                const bool &read_only = false,
                const bool &do_sign = false);


        bool isRead_only() const {
            return read_only;
        }

        const std::string &getSignature() const {
            return signature;
        }

        const tm &getLast_modified() const {
            return last_modified;
        }

        void upgrade_format(const std::string &newRoot ="");

        void destroy();

        void create();

        void addPackage(pkg::PackageInfo& pkg);

        void removePackage(const pkg::PackageInfo& pkg);

        pkg::PackageInfo getPackage(const std::string& name);

        std::vector<pkg::PackageInfo> getPackages(const std::vector<std::string>& names);

        bool exists();

        std::string path();

    };
};


#endif //PKG6_CATALOGPARTBASE_H
