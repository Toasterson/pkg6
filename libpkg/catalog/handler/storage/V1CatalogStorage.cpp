//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <catalog/handler/storage/V1CatalogStorage.h>
#include <catalog/handler/filestreamparser/V1BaseHandler.h>
#include <catalog/handler/filestreamparser/V1DependencySummaryHandler.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/filereadstream.h>
#include <fstream>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

pkg::PackageInfo pkg::V1CatalogStorage::streamLoadPackage(const string &fmri) {
    return pkg::PackageInfo();
}

int pkg::V1CatalogStorage::getFileSize() {
    return 0;
}

bool pkg::V1CatalogStorage::streamTransferPackages(pkg::ICatalogStorage &targetInterface) {
    try {
        for (std::string json_file : {"/catalog.base.C", "/catalog.dependency.C", "/catalog.summary.C"}) {
            Progress progress = Progress(
                    "importing pkg5 metadata from " + json_file,
                    "packages", getPackageVersionCount());

            if (json_file == "/catalog.base.C") {

            } else {

            }
            // The json files from pkg5 are huge one needs to read it with a stream reader.
            string file = statePath + json_file;
            FILE *fp = fopen(file.c_str(), "r");
            char readBuffer[65536];
            FileReadStream is(fp, readBuffer, sizeof(readBuffer));
            Reader catalog_reader;
            //TODO Re hook up Catalog FileHandler
            //catalog_reader.Parse(is, handler);
            fclose(fp);
        }
    } catch (...) {
        return false;
    }
    return true;
}

bool pkg::V1CatalogStorage::streamPackageExists(const string &fmri) {
    return false;
}

bool pkg::V1CatalogStorage::useStreamApproach() {
    return true;
}

bool pkg::V1CatalogStorage::does_apply() {
    return fs::exists((statePath + "/catalog.attrs").c_str());
}

int pkg::V1CatalogStorage::getPackageCount() {
    std::ifstream attrifstream(getAttrFilePath().c_str());
    IStreamWrapper isw(attrifstream);
    Document catalog_attrs;
    catalog_attrs.ParseStream(isw);
    return catalog_attrs["package-count"].GetUint();
}

int pkg::V1CatalogStorage::getPackageVersionCount() {
    std::ifstream attrifstream(getAttrFilePath().c_str());
    IStreamWrapper isw(attrifstream);
    Document catalog_attrs;
    catalog_attrs.ParseStream(isw);
    return catalog_attrs["package-version-count"].GetUint();
}

bool pkg::V1CatalogStorage::transferPackages(pkg::ICatalogStorage &targetInterface) {
    //Checkfilesize if above 20MB use FileStream Approach
    if(useStreamApproach()){
        return streamTransferPackages(targetInterface);
    }
    return false;
}

pkg::PackageInfo pkg::V1CatalogStorage::loadPackage(const string &fmri) {
    return pkg::PackageInfo();
}

bool pkg::V1CatalogStorage::removePackage(const pkg::PackageInfo &pkg) {
    return false;
}

bool pkg::V1CatalogStorage::addOrUpdatePackage(const pkg::PackageInfo &pkg) {
    return false;
}

bool pkg::V1CatalogStorage::updatePackage(const pkg::PackageInfo &updatePkg) {
    return false;
}

bool pkg::V1CatalogStorage::savePackage(const pkg::PackageInfo &pkg) {
    return false;
}

bool pkg::V1CatalogStorage::addPackage(const pkg::PackageInfo &pkg) {
    return false;
}

bool pkg::V1CatalogStorage::packageExists(const string &fmri) {
    return false;
}

FILE pkg::V1CatalogStorage::open() {
    return FILE();
}

bool pkg::V1CatalogStorage::close() {
    return false;
}

bool pkg::V1CatalogStorage::create() {
    return false;
}
