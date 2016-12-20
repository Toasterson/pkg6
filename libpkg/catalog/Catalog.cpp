//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "Catalog.h"
#include "CatalogError.h"
#include <catalog/handler/filestreamparser/V1BaseHandler.h>
#include <catalog/handler/filestreamparser/V1DependencySummaryHandler.h>


namespace fs = boost::filesystem;
using namespace rapidjson;

void pkg::Catalog::upgrade_format(ICatalogStorage &newInterface){
    read_only = false;
    //TODO Error handling

    newInterface.create();

}

pkg::Catalog::Catalog(const string &root, const string &name, const bool &read_only, const bool &do_sign):
    read_only{read_only},
    do_sign{do_sign},
    needs_upgrade{false}
{
    //If root_dir contains pkg5 metadata make catalog readonly thus requiring upgrade_format
    // As the Catalog in pkg6 will always reside on disk a load is not required

}

void pkg::Catalog::addPackage(pkg::PackageInfo &pkg) {
    if(!read_only) {
        //TODO Errorhandeling
        interface.addPackage(pkg);
    }
}

void pkg::Catalog::updatePackage(pkg::PackageInfo &updatePkg) {
    if(!read_only){
        //TODO Errorhandeling
        interface.updatePackage(updatePkg);
    }
}

void pkg::Catalog::addOrUpdatePackage(pkg::PackageInfo &pkg) {
    if(!read_only){
        //TODO Errorhandeling
        interface.addOrUpdatePackage(pkg);
    }
}

void pkg::Catalog::removePackage(pkg::PackageInfo &pkg) {
    if(!read_only) {
        //TODO Errorhandeling
        interface.removePackage(pkg);
    }
}

void pkg::Catalog::savePackage(pkg::PackageInfo &pkg) {
    if(!read_only){
        //TODO Errorhandeling
        interface.savePackage(pkg);
    }
}

void pkg::Catalog::loadPackage(pkg::PackageInfo &pkg) {
    if(interface.packageExists(pkg.getFmri())) {
        pkg = interface.loadPackage(pkg.getFmri());
    } else {
        throw new pkg::exception::PackageLoadException("Package "+pkg.getFmri()+" does not exist");
    }
}

pkg::PackageInfo pkg::Catalog::getPackage(const std::string &fmri) {
    /*
    //TODO Port the SAT solver from PKG5
    if(boost::starts_with(fmri, "pkg://")){
        pkg::PackageInfo pkg{fmri};
        loadPackage(pkg);
        return pkg;
    }
    string s_path{boost::erase_first_copy(fmri, string("pkg:/"))};
    std::vector<std::string> found;
    for(auto entry = fs::recursive_directory_iterator(statePath().c_str(), fs::symlink_option::no_recurse); entry != fs::recursive_directory_iterator(); entry++){
        if(boost::contains(entry->path().string(), s_path)){
            found.push_back(entry->path().string());
        }
    }
    if (found.size() == 0){
        throw pkg::exception::PackageNotExistException(fmri);
    }
    if(found.size() > 1){
        //Initialize this string as fmri without version of first entry in found
        string s_tmp_pkg_name{found[0].erase(found[0].find('@'), found[0].size())};
        for(auto entry: found){
            entry.erase(entry.find('@'), entry.size());
            if(s_tmp_pkg_name != entry){
                //If we have more than one fmri (version removed) than we can not resolve that partial fmri
                throw pkg::exception::PackageResolveException(fmri + " to many possible candidates");
            }
        }
        //TODO IPS version sorter
        //Take newest package instead of first
        std::sort(found.begin(), found.end());
    }
    //Then load Data to class.
    pkg::PackageInfo pkg("pkg://"+found[0]);
    loadPackage(pkg);
    return pkg;
     */
}

std::vector<pkg::PackageInfo> pkg::Catalog::getPackages(const std::vector<std::string> &fmris) {
    std::vector<pkg::PackageInfo> packages;
    for(auto fmri: fmris) {
        packages.push_back(getPackage(fmri));
    }
    return packages;
}

bool pkg::Catalog::contains(const pkg::PackageInfo &pkg) {
    return contains(pkg.getFmri());
}


bool pkg::Catalog::contains(std::string fmri) {
    return interface.packageExists(fmri);
}

bool pkg::Catalog::needsUpgrade() {
    return needs_upgrade;
}

