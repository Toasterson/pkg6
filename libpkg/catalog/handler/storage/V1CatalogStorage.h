//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//
#ifndef PKG6_V1CATALOGSTORAGE_H
#define PKG6_V1CATALOGSTORAGE_H
#include <interfaces/ICatalogStorage.h>
#include <Progress.h>

namespace pkg{
    class V1CatalogStorage : public ICatalogStorage{
    private:
        string statePath;
        string getAttrFilePath(){
            return statePath + "/catalog.attrs";
        }
        string getBaseFilePath(){
            return statePath + "/catalog.base.C";
        }
        string getDependencyFilePath(){
            return statePath + "/catalog.dependency.C";
        }
        string getSummaryFilePath(){
            return statePath + "/catalog.summary.C";
        }
        pkg::PackageInfo streamLoadPackage(const string &fmri);
        int getFileSize();
        bool streamTransferPackages(ICatalogStorage &targetInterface);
        bool streamPackageExists(const string &fmri);
        bool useStreamApproach();
    public:
        V1CatalogStorage(){}
        V1CatalogStorage(const string &root, const string &name) : ICatalogStorage(root, name), statePath{root+"/state/"+name} {}

        virtual FILE open();

        virtual bool close();

        virtual bool create();

        virtual bool does_apply();

        virtual int getPackageCount();

        virtual int getPackageVersionCount();

        virtual bool packageExists(const string &fmri);

        virtual bool addPackage(const pkg::PackageInfo &pkg);

        virtual bool savePackage(const pkg::PackageInfo &pkg);

        virtual bool updatePackage(const pkg::PackageInfo &updatePkg);

        virtual bool addOrUpdatePackage(const pkg::PackageInfo &pkg);

        virtual bool removePackage(const pkg::PackageInfo &pkg);

        virtual pkg::PackageInfo loadPackage(const string &fmri);

        virtual bool transferPackages(ICatalogStorage &targetInterface);

    };
}
#endif //PKG6_V1CATALOGSTORAGE_H