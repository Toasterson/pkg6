//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_ICATALOGACCESS_H
#define PKG6_ICATALOGACCESS_H
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/filereadstream.h>
#include <string>
#include <fstream>
#include <rapidjson/document.h>
#include <boost/filesystem.hpp>
#include <package/PackageInfo.h>

namespace fs = boost::filesystem;

using namespace std;
using namespace rapidjson;

// Interface for Low-level functions of the Catalog (Filesystem, TCP, HTTP)
// As interface in order to be able to Mock them out.
namespace pkg {
    class ICatalogStorage {
    public:
        virtual ~ICatalogStorage() {}

        explicit virtual ICatalogStorage(const string &root, const string &name) = 0;

        virtual bool createStatePath() = 0;

        virtual bool importLegacy(const string &file, const BaseReaderHandler &handler) = 0;

        virtual int legacyPackageCount(const string &file) = 0;

        virtual bool isLegacy() = 0;

        virtual bool packageExists(const string &fmri) = 0;

        virtual bool addPackage(const pkg::PackageInfo &pkg) = 0;

        virtual bool savePackage(const pkg::PackageInfo &pkg) = 0;

        virtual bool updatePackage(const pkg::PackageInfo &updatePkg) = 0;

        virtual bool addOrUpdatePackage(const pkg::PackageInfo &pkg) = 0;

        virtual bool removePackage(const pkg::PackageInfo &pkg) = 0;

        virtual pkg::PackageInfo loadPackage(const string &fmri) = 0;
    };
}
#endif //PKG6_ICATALOGACCESS_H
