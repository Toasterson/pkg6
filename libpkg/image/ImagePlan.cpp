//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "ImagePlan.h"

void pkg::ImagePlan::install() {
    //Get Manifest from url or local dir if already present

    //Get all the files for installation

    //Count all Actions (helper in package)

    //Call install on all installable actions


}

bool pkg::ImagePlan::contains(const pkg::PackageInfo &pkg) {
    return contains(pkg.getFmri());
}

bool pkg::ImagePlan::contains(const std::string &fmri) {
    for(auto pkg : packages){
        if(pkg.getFmri() == fmri){
            return true;
        }
    }
    return false;
}

void pkg::ImagePlan::add(const pkg::PackageInfo &pkg) {
    //TODO get Manifest from package and add actions to plan
    packages.push_back(pkg);
}

pkg::ImagePlan::ImagePlan(const std::vector<pkg::PackageInfo> &packages) {
    add(packages);
}

void pkg::ImagePlan::add(const std::vector<pkg::PackageInfo> &pkgs) {
    for(auto pkg: pkgs){
        add(pkg);
    }
}

