//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <boost/filesystem.hpp>
#include <package/PackageException.h>
#include "Image.h"
#include <vectoradd.h>

namespace fs = boost::filesystem;

std::string pkg::Image::getImgDir(){
    return image_root+IMAGE_ROOT_PATH;
}

std::string pkg::Image::getImgRoot() {
    return image_root;
}


bool pkg::Image::isLocked() {
    return locked;
}

std::string pkg::Image::getWriteCachePath() {
    return "";
}

std::string pkg::Image::getArch() {
    return config.getVariant("variant.arch");
}

pkg::Image::Image(const std::string &root, const bool &allow_ondisk_upgrade):
    image_root(root),
    allow_ondisk_upgrade(allow_ondisk_upgrade),
    config(pkg::ImageConfig(getImgDir())),
    history(pkg::history::History(getImgDir()+"/history")),
    installed(pkg::Catalog(getImgDir(), CATALOG_INSTALLED)),
    known(pkg::Catalog(getImgDir(), CATALOG_KNOWN))
{
    if(known.needsUpgrade() or installed.needsUpgrade()){
        this->upgrade_needed = true;
    }
}

void pkg::Image::lock(const bool &allow_unprevileged) {

}

void pkg::Image::unlock() {

}

int pkg::Image::getImageType() {
    return 0;
}

void pkg::Image::create(const bool &is_zone, const std::vector<std::string> &publishers, const bool &refresh_allowed,
                        const std::map<std::string, std::string> &properties,
                        const std::map<std::string, std::string> &variants,
                        const std::map<std::string, std::string> &facets) {

}

std::tm pkg::Image::getLastModified() {
    return tm();
}

void pkg::Image::upgrade_format(std::string newRoot) {
    if(newRoot == ""){
        //TODO When we get libbe support add call to make new be and set image_root to new be + IMAGE_ROOT_PATH allow_ondisk_upgrade then says if we make new be or not for now we don't
        cerr << "-R must be provided libbe support currently not developed";
        throw std::exception();
    }
    newRoot += IMAGE_ROOT_PATH;
    if(!fs::is_directory(fs::system_complete(newRoot))){
        fs::create_directories(fs::system_complete(newRoot));
    }
    //TODO add IMAGE_ROOT_PATH to newRoot Path if we are a full image.

    history.upgrade_format(newRoot);
    config.upgrade_format(newRoot);
    installed.upgrade_format(newRoot);
    known.upgrade_format(newRoot);
}

bool pkg::Image::needsUpgrade() {
    return upgrade_needed;
}

pkg::ImagePlan pkg::Image::makePlan(const std::vector<std::string> &packages) {
    std::vector<pkg::PackageInfo> resolved = known.getPackages(packages);
    pkg::ImagePlan plan(config, getImgDir()+"/cache");
    for(auto pkg : resolved){
        if(!installed.contains(pkg) and !plan.contains(pkg)){
            getNotInstalledDeps(pkg, plan);
            plan.add(pkg);
        }
    }
    return plan;
}

void pkg::Image::getNotInstalledDeps(const pkg::PackageInfo &pkg, pkg::ImagePlan &plan) {
    for(auto dep : pkg.dependencies){
        if(!installed.contains(dep.fmri) and !plan.contains(dep.fmri)){
            pkg::PackageInfo pack(dep.fmri);
            known.loadPackage(pack);
            getNotInstalledDeps(pack, plan);
        }
    }
}






























