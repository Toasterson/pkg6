//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "Catalog.h"
#include "CatalogError.h"
#include <algorithm>


namespace fs = boost::filesystem;
using namespace rapidjson;

void pkg::Catalog::upgrade_format(){
    read_only = false;
    //TODO Error handling
    oldinterface->transferPackages(v2interface);
}

pkg::Catalog::Catalog(const string &root, const string &name, const bool &read_only, const bool &do_sign):
    root{root},
    name{name},
    read_only{read_only},
    do_sign{do_sign},
    needs_upgrade{false},
    v1interface{V1CatalogStorage(root, name)},
    v2interface{V2CatalogStorage(root, name)},
    interface{&this->v2interface},
    oldinterface{&this->v1interface}
{
    //If root_dir contains pkg5 metadata make catalog readonly thus requiring upgrade_format
    // As the Catalog in pkg6 will always reside on disk a load is not required
    if (!interface->does_apply()){
        needs_upgrade = true;
        this->read_only = true;
    }
}

void pkg::Catalog::addPackage(pkg::PackageInfo &pkg) {
    if(!read_only) {
        //TODO Errorhandeling
        interface->addPackage(pkg);
    }
}

void pkg::Catalog::updatePackage(pkg::PackageInfo &updatePkg) {
    if(!read_only){
        //TODO Errorhandeling
        interface->updatePackage(updatePkg);
    }
}

void pkg::Catalog::addOrUpdatePackage(pkg::PackageInfo &pkg) {
    if(!read_only){
        //TODO Errorhandeling
        interface->addOrUpdatePackage(pkg);
    }
}

void pkg::Catalog::removePackage(pkg::PackageInfo &pkg) {
    if(!read_only) {
        //TODO Errorhandeling
        interface->removePackage(pkg);
    }
}

void pkg::Catalog::savePackage(pkg::PackageInfo &pkg) {
    if(!read_only){
        //TODO Errorhandeling
        interface->savePackage(pkg);
    }
}

pkg::PackageInfo pkg::Catalog::loadPackage(const std::string& fmri) {
    if(interface->packageExists(fmri)) {
        return interface->loadPackage(fmri);
    } else {
        throw new pkg::exception::PackageLoadException("Package "+fmri+" does not exist");
    }
}

pkg::PackageInfo pkg::Catalog::getPackage(const std::string &fmri) {

    //TODO Port the SAT solver from PKG5
    if(boost::starts_with(fmri, "pkg://")){
        return interface->loadPackage(fmri);
    }
    string fmriInternal{boost::erase_first_copy(fmri, string("pkg://"))};
    fmriInternal = boost::erase_first_copy(fmriInternal, string("pkg:/"));

    //TODO Throw error when we ask for regex Kind of Packages
    return resolve(fmriInternal);
}

std::vector<pkg::PackageInfo> pkg::Catalog::getPackages(const std::vector<std::string> &fmris) {
    std::vector<pkg::PackageInfo> packages;
    for(auto fmri: fmris) {
        packages.push_back(getPackage(fmri));
    }
    return packages;
}

bool pkg::Catalog::packageExists(const std::string &fmri) {
    return interface->packageExists(fmri);
}

bool pkg::Catalog::needsUpgrade() {
    return needs_upgrade;
}

void Catalog::create() {
    interface->create();
}

pkg::PackageInfo Catalog::resolve(const std::string &partialFmri) {
    //First of all lets determine if we have the Publisher in the fmri or not.
    string s_assumedPublisher{partialFmri.substr(0, partialFmri.find("/"))};
    if(!interface->hasPublisher(s_assumedPublisher)){
        //Put Publisher infront of fmri if we do not have the publisher specified
        //and try the function again
        //if we have summary set in the package we can asume we have found a match and return
        //TODO use prefered publisher first
        for(auto publisher : interface->getPublishers()){
            PackageInfo pkgTmp = resolve(publisher + "/" + partialFmri);
            if(!pkgTmp.summary.empty()){
                return pkgTmp;
            }
        }
    }
    //From here on we can assume we have publisher set

    //If we have the Package return it
    //Implementation should be version neutral
    if(packageExists(partialFmri)){
        return loadPackage(partialFmri);
    }

    //If we do not have the exact package lets see if only the version is Missing
    vector<string> packageNames = interface->getAllPackageNames();
    if(find(packageNames.begin(), packageNames.end(), partialFmri) != packageNames.end()){

    }

    //As last resort use a sat solver to expand something like nginx into web/server/nginx
    //Which considering the normal use case of pkg will be the default.


    return PackageInfo();
}

vector<pkg::PackageInfo> Catalog::resolve(const vector<string> &partialFmris) {
    vector<pkg::PackageInfo> packages;
    for(auto partFmri : partialFmris){
        packages.push_back(resolve(partFmri));
    }
    return packages;
}

