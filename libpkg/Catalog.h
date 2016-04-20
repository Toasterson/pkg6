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
        const std::string KNOWN_CATALOG_FILENAME = "known.catalog";
        const std::string INSTALLED_CATALOG_FILENAME = "installed.catalog";
        const std::string GENERIC_CATALOG_FILENAME = "catalog.info";

    private:
        bool read_only;
        bool do_sign;
        std::string signature;
        std::string root_dir;
        std::tm last_modified;
        std::string catalogtype;
        boost::property_tree::ptree data;
        void importpkg5();
    public:

        Catalog(const std::string &root = "",
                const std::string &catalogtype = "generic",
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

        bool exists();

        void load();

        std::string fullpath();

        void save();

        void finalize();


    };
};


#endif //PKG6_CATALOGPARTBASE_H
