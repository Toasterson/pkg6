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
#include <catalog/handler/storage/V2CatalogStorage.h>
#include <catalog/handler/storage/V1CatalogStorage.h>

namespace pkg {

    class Catalog {

    private:
        bool read_only;
        bool do_sign;
        bool needs_upgrade;
        std::string signature;
        std::tm last_modified;
        V1CatalogStorage v1interface;
        V2CatalogStorage v2interface;
        ICatalogStorage* oldinterface;
        ICatalogStorage* interface;
        string root;
        string name;
        //TODO set Prefered Publisher Function
    public:

        Catalog(const string &root,
                const string &name,
                const bool &read_only = false,
                const bool &do_sign = false);

        Catalog(ICatalogStorage &catalogStorage, ICatalogStorage &oldCatalogStorage):
                name{""},
                root{""},
                interface{&catalogStorage},
                oldinterface{&oldCatalogStorage} {}

        void upgrade_format();


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

        pkg::PackageInfo loadPackage(const std::string &fmri);

        pkg::PackageInfo getPackage(const std::string& fmri);

        pkg::PackageInfo resolve(const std::string &partialFmri);

        std::vector<pkg::PackageInfo> getPackages(const std::vector<std::string>& fmris);

        bool packageExists(const std::string &fmri);

        bool needsUpgrade();

        vector<pkg::PackageInfo> resolve(const vector<string> &partialFmris);

    };
};


#endif //PKG6_CATALOG_H
