//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <interfaces/ICatalogStorage.h>
#include <catalog/handler/filestreamparser/V1BaseHandler.h>
#include <catalog/handler/filestreamparser/V1DependencySummaryHandler.h>
#include <ctime>

namespace pkg {
    class V2CatalogStorage : public ICatalogStorage {
    private:
        string statePath;

        string filePath(const pkg::PackageInfo &pkg) const {
            return statePath + "/" + pkg.publisher + "/" + pkg.name + ".json";
        }

        fs::path pkg_directory(const pkg::PackageInfo &pkg) const {
            return fs::system_complete(filePath(pkg)).parent_path();
        }

        string FMRIVersionPart(const pkg::PackageInfo &pkg) const {
            char strtime[16];
            strftime(&strtime, 16, "%Y%m%dT%H%M%SZ", &pkg.packaging_date);
            return pkg.version + "," + pkg.build_release + "-" + pkg.branch + ":" + strtime;
        }

    public:
        V2CatalogStorage(){}
        explicit V2CatalogStorage(const string &root, const string &name) :
                ICatalogStorage(root, name), statePath{root+"/state/"+name} {}

        virtual bool create() {
            if (!fs::is_directory(fs::system_complete(statePath))) {
                return fs::create_directories(fs::system_complete(statePath));
            }
            return false;
        }

        virtual bool packageExists(const string &fmri) {
            if (boost::starts_with(fmri, "pkg://")) {
                boost::erase_first(fmri, "pkg://");
            } else if (boost::starts_with(fmri, "pkg:/")) {
                boost::erase_first(fmri, "pkg:/");
            }
            //Lets first try if path exists e.g. publisher is already prepended
            if (fs::exists((statePath + "/" + fmri).c_str())) {
                return true;
            }
            //If the path does not exist prepend the any publisher to see if it does
            for (fs::directory_iterator directoryIterator(fs::path(statePath.c_str()));
                 directoryIterator != fs::directory_iterator(); directoryIterator++) {
                if (fs::exists(directoryIterator->path().string() + "/" + fmri)) {
                    return true;
                }
            }
            return false;
        }

        virtual bool addPackage(const pkg::PackageInfo &pkg) {
            try {
                //Write Directory Structure of Package if does not exist
                fs::path pkg_path = pkg_directory(pkg);
                if (!fs::is_directory(pkg_path)) {
                    fs::create_directories(pkg_path);

                }
                //Then save the Contents of Packge
                savePackage(pkg);
            } catch (...) {
                return false;
            }
            return true;
        }

        virtual bool savePackage(const pkg::PackageInfo &pkg) {
            //Save Package to disk as one json per package
            try {
                Document doc;
                ifstream ifs(filePath(pkg));
                IStreamWrapper isw(ifs);
                doc.ParseStream(isw);
                ifs.close();
                GenericValue val;
                pkg.Serialize(val);
                doc[FMRIVersionPart(pkg).c_str()] = val;
                ofstream ofs(filePath(pkg));
                OStreamWrapper osw(ofs);
                Writer<OStreamWrapper> fileWriter;
                doc.Accept(fileWriter);
            } catch (...){
                return false;
            }
            return true;
        }

        virtual bool updatePackage(const pkg::PackageInfo &updatePkg) {
            try {
                //Load from disk
                pkg::PackageInfo pkg = loadPackage(updatePkg.getFmri());
                //Use += to merge with second pkg instance overwriting first
                pkg += updatePkg;
                //Save New Package data
                savePackage(pkg);
            } catch (...) {
                return false;
            }
            return true;
        }

        virtual bool addOrUpdatePackage(const pkg::PackageInfo &pkg) {
            try {
                if (packageExists(pkg.getFmri())){
                    updatePackage(pkg);
                } else {
                    addPackage(pkg);
                }
            } catch (...) {
                return false;
            }
            return true;
        }

        virtual bool removePackage(const pkg::PackageInfo &pkg) {
            try {
                Document doc;
                ifstream ifs(filePath(pkg));
                IStreamWrapper isw(ifs);
                doc.ParseStream(isw);
                ifs.close();
                doc.EraseMember(FMRIVersionPart(pkg).c_str());
                ofstream ofs(filePath(pkg));
                OStreamWrapper osw(ofs);
                Writer<OStreamWrapper> fileWriter;
                doc.Accept(fileWriter);
            } catch (...) {
                return false;
            }
            return true;
        }

        virtual pkg::PackageInfo loadPackage(const string fmri) {
            pkg::PackageInfo pkg;
            ifstream ifs(filePath(pkg));
            IStreamWrapper isw(ifs);
            Document doc;
            doc.ParseStream(isw);
            ifs.close();
            pkg.Deserialize(doc[FMRIVersionPart(pkg).c_str()]);
            return pkg;
        }

    };

}