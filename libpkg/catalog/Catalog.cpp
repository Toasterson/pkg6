//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "Catalog.h"
#include <rapidjson/filereadstream.h>
#include <catalog/parser/V1BaseHandler.h>
#include <catalog/parser/V1DependencySummaryHandler.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/ostreamwrapper.h>

namespace fs = boost::filesystem;
using namespace boost::property_tree;
using namespace rapidjson;

void pkg::Catalog::upgrade_format(const std::string &newRoot) {
    std::string importDir = root_dir + "/state/" + name;
    root_dir = newRoot;
    read_only = false;
    if(!fs::is_directory(fs::system_complete(statePath()))){
        fs::create_directories(fs::system_complete(statePath()));
    }
    // The json files from pkg5 are huge one needs to read it with a stream reader.

    std::ifstream attrifstream((importDir + "/catalog.attrs").c_str());
    IStreamWrapper isw(attrifstream);
    Document catalog_attrs;
    catalog_attrs.ParseStream(isw);
    int package_count = catalog_attrs["package-version-count"].GetUint();

    for(std::string json_file : {"/catalog.base.C", "/catalog.dependency.C", "/catalog.summary.C"}) {
        Progress progress = Progress("importing pkg5 metadata from "+ json_file +" into catalog " + name, "packages", package_count);
        FILE *fp = fopen((importDir + json_file).c_str(), "r");
        char readBuffer[65536];
        FileReadStream is(fp, readBuffer, sizeof(readBuffer));
        if(json_file == "/catalog.base.C"){
            //TODO Handler for summary
            V1BaseHandler handler = V1BaseHandler(*this, progress);
            Reader catalog_reader;
            catalog_reader.Parse(is, handler);
        } else {
            V1DependencySummaryHandler handler = V1DependencySummaryHandler(*this, progress);
            Reader catalog_reader;
            catalog_reader.Parse(is, handler);
        }
        fclose(fp);
    }
}

pkg::Catalog::Catalog(const std::string &root, const std::string &name, const bool &read_only, const bool &do_sign):
    root_dir(root),
    name(name),
    read_only(read_only),
    do_sign(do_sign)
{
    //If root_dir contains pkg5 metadata make catalog readonly thus requiring upgrade_format
    // As the Catalog in pkg6 will always reside on disk a load is not required
    if(fs::is_regular_file(fs::system_complete((root_dir+"/catalog.attrs").c_str()))){
        this->read_only = true;
        this->needs_upgrade = true;
    }
}

void pkg::Catalog::addPackage(pkg::PackageInfo &pkg) {
    if(!read_only) {
        //Write Directory Structure of Package if does not exist
        fs::path pkg_path = fs::system_complete((statePath() + "/" + pkg.getFilePath()).c_str()).parent_path();
        if (!fs::is_directory(pkg_path)) {
            fs::create_directories(pkg_path);
        }
        savePackage(pkg);
    }
}

void pkg::Catalog::updatePackage(pkg::PackageInfo &updatePkg) {
    if(!read_only){
        //Make new Object with minimal data from updatePkg
        PackageInfo pkg(updatePkg);
        //Load from disk
        loadPackage(pkg);
        //Use += to merge with second pkg instance overwriting first
        pkg += updatePkg;
        //Save New Package data
        savePackage(pkg);
    }
}

void pkg::Catalog::addOrUpdatePackage(pkg::PackageInfo &pkg) {
    if(fs::is_directory(fs::system_complete((statePath() + "/" + pkg.getFilePath()).c_str()).parent_path())){
        updatePackage(pkg);
    } else {
        addPackage(pkg);
    }
}

void pkg::Catalog::removePackage(pkg::PackageInfo &pkg) {
    if(!read_only) {
        fs::path pkg_path = fs::system_complete((statePath() + "/" + pkg.getFilePath()).c_str()).parent_path();
        if (fs::is_directory(pkg_path)) {
            fs::remove_all(pkg_path);
        }
    }
}

void pkg::Catalog::savePackage(pkg::PackageInfo &pkg) {
    //Save Package to disk as one json per package
    ofstream ofs(statePath()+"/"+pkg.getFilePath());
    OStreamWrapper osw(ofs);
    Writer<OStreamWrapper> writer(osw);
    pkg.Serialize(writer);
}

void pkg::Catalog::loadPackage(pkg::PackageInfo &pkg) {
    ifstream ifs(statePath()+"/"+pkg.getFilePath());
    IStreamWrapper isw(ifs);
    Document doc;
    doc.ParseStream(isw);
    pkg.Deserialize(doc);
}

pkg::PackageInfo pkg::Catalog::getPackage(const std::string &fmri) {
    std::vector<std::string> found;
    for(auto entry = fs::recursive_directory_iterator(statePath(), fs::symlink_option::no_recurse); entry <= fs::recursive_directory_iterator(); entry++){
        if(boost::starts_with(entry->path().filename(), fmri)){
            found.push_back(entry->path().string());
        }
    }
    if(found.size() > 1){
        std::sort(found.begin(), found.end());
    }
    pkg::PackageInfo pkg("pkg://"+found[0]);
    loadPackage(pkg);
    return pkg::PackageInfo();
}

std::vector<pkg::PackageInfo> pkg::Catalog::getPackages(const std::vector<std::string> &fmris) {
    std::vector<pkg::PackageInfo> packages;
    for(auto fmri: fmris) {
        packages.push_back(getPackage(fmri));
    }
    return packages;
}

std::string pkg::Catalog::statePath() {
    return root_dir + "/state/" + name;
}

std::vector<pkg::PackageInfo> pkg::Catalog::dependResolve(const pkg::PackageInfo &pkg) {
    return vector<pkg::PackageInfo>();
}

bool pkg::Catalog::contains(const pkg::PackageInfo &pkg) {
    //Todo Implement
    return false;
}

bool pkg::Catalog::needsUpgrade() {
    return needs_upgrade;
}
