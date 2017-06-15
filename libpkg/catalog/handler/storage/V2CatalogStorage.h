//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//
#ifndef PKG6_V2CATALOGSTORAGE_H
#define PKG6_V2CATALOGSTORAGE_H
#include <interfaces/ICatalogStorage.h>
#include <catalog/handler/filestreamparser/V1BaseHandler.h>
#include <catalog/handler/filestreamparser/V1DependencySummaryHandler.h>
#include <ctime>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

namespace fs = boost::filesystem;

namespace pkg {
    class V2CatalogStorage : public ICatalogStorage {
    private:
        string statePath;

        string filePath(const pkg::PackageInfo &pkg) const;

        string filePath(string partialFmri);

        fs::path pkg_directory(const pkg::PackageInfo &pkg) const;

        string FMRIVersionPart(const pkg::PackageInfo &pkg) const;

        bool FMRICompare(const string &fmri1, const string &fmri2);

    public:
        V2CatalogStorage(){}
        explicit V2CatalogStorage(const string &root, const string &name) :
                ICatalogStorage(root, name), statePath{root+"/state/"+name} {}

        virtual FILE open();

        //Same as open
        virtual bool close();

        virtual bool create();

        virtual bool does_apply();

        virtual int getPackageCount();

        virtual int getPackageVersionCount();

        virtual bool packageExists(const string &fmri);

        virtual bool hasPublisher(const string& publisher);

        virtual vector<string> getPublishers();

        virtual vector<string> getAllPackageNames();

        virtual pkg::PackageInfo getNewestPackage(const string &fmri);

        virtual bool addPackage(const pkg::PackageInfo &pkg);

        virtual bool savePackage(const pkg::PackageInfo &pkg);

        virtual bool updatePackage(const pkg::PackageInfo &updatePkg);

        virtual bool addOrUpdatePackage(const pkg::PackageInfo &pkg);

        virtual bool removePackage(const pkg::PackageInfo &pkg);

        virtual pkg::PackageInfo loadPackage(const string &fmri);

        virtual pkg::PackageInfo loadNewestPackageVersion(const string &partialFmri);

        virtual bool transferPackages(ICatalogStorage &targetInterface);

    };

}
#endif //PKG6_V2CATALOGSTORAGE_H