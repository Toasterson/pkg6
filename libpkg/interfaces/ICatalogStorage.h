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
        ICatalogStorage(){}
        explicit ICatalogStorage(const string &root, const string &name){}

        //In Case any Filehandles etc. need to be opened. Used only Internally. But Public for use and Mocking
        virtual FILE open() = 0;

        //Same as open
        virtual bool close() = 0;

        //Create an empty Catalog
        virtual bool create() = 0;

        virtual int getPackageCount() = 0;

        virtual int getPackageVersionCount() = 0;

        virtual bool packageExists(const string &fmri) = 0;

        virtual bool addPackage(const pkg::PackageInfo &pkg) = 0;

        virtual bool savePackage(const pkg::PackageInfo &pkg) = 0;

        virtual bool updatePackage(const pkg::PackageInfo &updatePkg) = 0;

        virtual bool addOrUpdatePackage(const pkg::PackageInfo &pkg) = 0;

        virtual bool removePackage(const pkg::PackageInfo &pkg) = 0;

        virtual pkg::PackageInfo loadPackage(const string &fmri) = 0;

        virtual bool transferPackages(ICatalogStorage &targetInterface) = 0;
    };
}
#endif //PKG6_ICATALOGACCESS_H
