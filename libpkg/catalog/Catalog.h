//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_CATALOG_H
#define PKG6_CATALOG_H

#include <string>
#include <chrono>
#include <map>
#include "package/PackageInfo.h"
#include <Progress.h>
#define BOOST_SYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>
#include <interfaces/ICatalogStorage.h>

namespace pkg {

    class Catalog {

    private:
        bool read_only;
        bool do_sign;
        bool needs_upgrade;
        std::string signature;
        std::tm last_modified;
        ICatalogStorage interface;

    public:

        Catalog(const ICatalogStorage &iCatalogStorage,
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

        void upgrade_format(const ICatalogStorage &iCatalogStorage);

        void destroy();

        void create();

        void addPackage(pkg::PackageInfo& pkg);

        void updatePackage(pkg::PackageInfo& updatePkg);

        void addOrUpdatePackage(pkg::PackageInfo& pkg);

        void removePackage(pkg::PackageInfo& pkg);

        void savePackage(pkg::PackageInfo& pkg);

        void loadPackage(pkg::PackageInfo& pkg);

        pkg::PackageInfo getPackage(const std::string& fmri);

        std::vector<pkg::PackageInfo> getPackages(const std::vector<std::string>& fmris);

        bool contains(const pkg::PackageInfo& pkg);

        bool contains(std::string fmri);

        bool needsUpgrade();

    };
};


#endif //PKG6_CATALOGPARTBASE_H
