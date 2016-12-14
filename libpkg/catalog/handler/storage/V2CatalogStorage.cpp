//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <interfaces/ICatalogStorage.h>
#include <catalog/handler/filestreamparser/V1BaseHandler.h>
#include <catalog/handler/filestreamparser/V1DependencySummaryHandler.h>
namespace pkg {
    class V2CatalogStorage : public ICatalogStorage {
    private:
        string statePath;
    public:
        explicit virtual V2CatalogStorage(const string &root, const string &name) :
                ICatalogStorage(root, name), statePath{root+"/state/"+name} {}

        Document Parse(const ifstream &ifstream) {
            IStreamWrapper isw(ifstream);
            Document doc;
            doc.ParseStream(isw);
            return doc;
        }

        bool Write(const ofstream &stream, const pkg::PackageInfo &pkg) {
            try {
                OStreamWrapper osw(stream);
                Writer<OStreamWrapper> writer(osw);
                pkg.Serialize(writer);
            } catch (...) {
                return false;
            }
            return true;
        }

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
                fs::path pkg_path = fs::system_complete((statePath + "/" + pkg.getFilePath()).c_str()).parent_path();
                if (!fs::is_directory(pkg_path)) {
                    fs::create_directories(pkg_path);
                }
                savePackage(pkg);
            } catch (...) {
                return false;
            }
            return true;
        }

        virtual bool savePackage(const pkg::PackageInfo &pkg) {
            //Save Package to disk as one json per package
            return Write(ofstream(statePath + "/" + pkg.getFilePath()), pkg);
        }

        virtual bool updatePackage(const pkg::PackageInfo &updatePkg) {
            try {
                //Load from disk
                pkg::PackageInfo pkg = loadPackage(updatePkg.getFilePath());
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
                if (fs::is_directory(
                        fs::system_complete((statePath + "/" + pkg.getFilePath()).c_str()).parent_path())) {
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
                fs::path pkg_path = fs::system_complete((statePath + "/" + pkg.getFilePath()).c_str()).parent_path();
                if (fs::is_directory(pkg_path)) {
                    fs::remove_all(pkg_path);
                }
            } catch (...) {
                return false;
            }
            return true;
        }

        virtual pkg::PackageInfo loadPackage(const string fmri) {
            ifstream ifs(statePath + "/" + fmri + ".json");
            IStreamWrapper isw(ifs);
            Document doc;
            doc.ParseStream(isw);
            pkg::PackageInfo pkg;
            pkg.Deserialize(doc);
            return pkg;
        }

    };

}