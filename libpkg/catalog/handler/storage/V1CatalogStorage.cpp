//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <interfaces/ICatalogStorage.h>
#include <Progress.h>
#include <catalog/handler/filestreamparser/V1BaseHandler.h>
#include <catalog/handler/filestreamparser/V1DependencySummaryHandler.h>

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
        pkg::PackageInfo streamLoadPackage(const string &fmri){

        }
        int getFileSize(){

        }
        bool streamTransferPackages(ICatalogStorage &targetInterface){
            try {
                for (std::string json_file : {"/catalog.base.C", "/catalog.dependency.C", "/catalog.summary.C"}) {
                    Progress progress = Progress(
                            "importing pkg5 metadata from " + json_file,
                            "packages", getPackageVersionCount());
                    BaseReaderHandler handler;
                    if (json_file == "/catalog.base.C") {
                        handler = V1BaseHandler(*this, progress);
                    } else {
                        handler = V1DependencySummaryHandler(*this, progress);
                    }
                    // The json files from pkg5 are huge one needs to read it with a stream reader.
                    string file = statePath + json_file;
                    FILE *fp = fopen(file.c_str(), "r");
                    char readBuffer[65536];
                    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
                    Reader catalog_reader;
                    catalog_reader.Parse(is, handler);
                    fclose(fp);
                }
            } catch (...) {
                return false;
            }
            return true;
        }
        bool streamPackageExists(const string &fmri){

        }
        bool useStreamApproach(){
            return true;
        }
    public:
        V1CatalogStorage(const string &root, const string &name) : ICatalogStorage(root, name), statePath{root+"/state/"+name} {}

        virtual FILE open() = 0;

        virtual bool close() = 0;

        virtual bool create() = 0;

        virtual int getPackageCount(){
            std::ifstream attrifstream(getAttrFilePath().c_str());
            IStreamWrapper isw(attrifstream);
            Document catalog_attrs;
            catalog_attrs.ParseStream(isw);
            return catalog_attrs["package-count"].GetUint();
        }

        virtual int getPackageVersionCount(){
            std::ifstream attrifstream(getAttrFilePath().c_str());
            IStreamWrapper isw(attrifstream);
            Document catalog_attrs;
            catalog_attrs.ParseStream(isw);
            return catalog_attrs["package-version-count"].GetUint();
        }

        virtual bool packageExists(const string &fmri) = 0;

        virtual bool addPackage(const pkg::PackageInfo &pkg) = 0;

        virtual bool savePackage(const pkg::PackageInfo &pkg) = 0;

        virtual bool updatePackage(const pkg::PackageInfo &updatePkg) = 0;

        virtual bool addOrUpdatePackage(const pkg::PackageInfo &pkg) = 0;

        virtual bool removePackage(const pkg::PackageInfo &pkg) = 0;

        virtual pkg::PackageInfo loadPackage(const string &fmri){

        }

        virtual bool transferPackages(ICatalogStorage &targetInterface){
            //Checkfilesize if above 20MB use FileStream Approach
            if(useStreamApproach()){
                return streamTransferPackages(targetInterface);
            }
            return false;
        }

    };
}