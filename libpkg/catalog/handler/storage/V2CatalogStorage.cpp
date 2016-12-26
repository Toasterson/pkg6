//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <catalog/handler/storage/V2CatalogStorage.h>
#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <boost/filesystem/operations.hpp>
#include "JSONPackageSerializer.h"

namespace fs = boost::filesystem;

bool pkg::V2CatalogStorage::addOrUpdatePackage(const pkg::PackageInfo &pkg) {
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

bool pkg::V2CatalogStorage::removePackage(const pkg::PackageInfo &pkg) {
    try {
        Document doc;
        ifstream ifs(filePath(pkg));
        IStreamWrapper isw(ifs);
        doc.ParseStream(isw);
        ifs.close();
        doc.EraseMember(FMRIVersionPart(pkg).c_str());
        ofstream ofs(filePath(pkg));
        OStreamWrapper osw(ofs);
        Writer<OStreamWrapper> fileWriter(osw);
        doc.Accept(fileWriter);
    } catch (...) {
        return false;
    }
    return true;
}

bool pkg::V2CatalogStorage::updatePackage(const pkg::PackageInfo &updatePkg) {
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

bool pkg::V2CatalogStorage::savePackage(const pkg::PackageInfo &pkg) {
    //Save Package to disk as one json per package
    try {
        Document doc;
        ifstream ifs(filePath(pkg));
        IStreamWrapper isw(ifs);
        doc.ParseStream(isw);
        ifs.close();
        Value val(kObjectType);
        JSONPackageSerializer ser;
        ser.Serialize(pkg, val);
        doc[FMRIVersionPart(pkg).c_str()] = val;
        ofstream ofs(filePath(pkg));
        OStreamWrapper osw(ofs);
        Writer<OStreamWrapper> fileWriter(osw);
        doc.Accept(fileWriter);
    } catch (...){
        return false;
    }
    return true;
}

bool pkg::V2CatalogStorage::addPackage(const pkg::PackageInfo &pkg) {
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

bool pkg::V2CatalogStorage::packageExists(const string &fmri) {
    string fmritmp = fmri;
    if (boost::starts_with(fmritmp, "pkg://")) {
        boost::erase_first(fmritmp, "pkg://");
    } else if (boost::starts_with(fmritmp, "pkg:/")) {
        boost::erase_first(fmritmp, "pkg:/");
    }
    //Lets first try if path exists e.g. publisher is already prepended
    if (fs::exists((statePath + "/" + fmritmp).c_str())) {
        return true;
    }
    //If the path does not exist prepend the any publisher to see if it does
    for (fs::directory_iterator directoryIterator(fs::path(statePath.c_str()));
         directoryIterator != fs::directory_iterator(); directoryIterator++) {
        if (fs::exists(directoryIterator->path().string() + "/" + fmritmp)) {
            return true;
        }
    }
    return false;
}

bool pkg::V2CatalogStorage::does_apply() {
    return fs::exists((statePath + "/catalog.json").c_str());
}

bool pkg::V2CatalogStorage::create() {
    if (!fs::is_directory(fs::system_complete(statePath))) {
        return fs::create_directories(fs::system_complete(statePath));
    }
    return false;
}

string pkg::V2CatalogStorage::FMRIVersionPart(const pkg::PackageInfo &pkg) const {
    char strtime;
    strftime(&strtime, 16, "%Y%m%dT%H%M%SZ", &pkg.packaging_date);
    return pkg.version + "," + pkg.build_release + "-" + pkg.branch + ":" + strtime;
}

fs::path pkg::V2CatalogStorage::pkg_directory(const pkg::PackageInfo &pkg) const {
    return fs::system_complete(filePath(pkg)).parent_path();
}

string pkg::V2CatalogStorage::filePath(const pkg::PackageInfo &pkg) const {
    return statePath + "/" + pkg.publisher + "/" + pkg.name + ".json";
}

FILE pkg::V2CatalogStorage::open() {
    return FILE();
}

bool pkg::V2CatalogStorage::close() {
    return false;
}

bool pkg::V2CatalogStorage::transferPackages(pkg::ICatalogStorage &targetInterface) {
    return false;
}

int pkg::V2CatalogStorage::getPackageVersionCount() {
    return 0;
}

int pkg::V2CatalogStorage::getPackageCount() {
    return 0;
}

pkg::PackageInfo pkg::V2CatalogStorage::loadPackage(const string &fmri) {
    pkg::PackageInfo pkg(fmri);
    ifstream ifs(filePath(pkg));
    IStreamWrapper isw(ifs);
    Document doc;
    doc.ParseStream(isw);
    ifs.close();
    JSONPackageSerializer deser;
    pkg += deser.Deserialize(doc[FMRIVersionPart(pkg).c_str()]);
    return pkg;
}
