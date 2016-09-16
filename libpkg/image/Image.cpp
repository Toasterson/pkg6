//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <boost/filesystem.hpp>
#include <package/PackageException.h>
#include "Image.h"
#include <vectoradd.h>
#include <http_client/HttpClient.h>

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
    return image_root + "/cache";
}

std::string pkg::Image::getArch() {
    return config.getVariant("arch");
}

pkg::Image::Image(const std::string &root, const bool &allow_ondisk_upgrade):
    image_root(root),
    allow_ondisk_upgrade(allow_ondisk_upgrade),
    config(pkg::ImageConfig(getImgDir())),
    history(pkg::history::History(getImgDir()+"/history")),
    installed(pkg::Catalog(getImgDir(), CATALOG_INSTALLED)),
    known(pkg::Catalog(getImgDir(), CATALOG_KNOWN)),
    locked{false},
    blocking_locks{false},
    version{6}
{
    if(known.needsUpgrade() or installed.needsUpgrade() or config.needs_upgrade){
        this->upgrade_needed = true;
    }
}

void pkg::Image::lock(const bool &allow_unprevileged) {
    locked = true;
    ofstream ofs((getImgRoot()+"/locked").c_str());
    ofs << locked;
    ofs.close();
}

void pkg::Image::unlock() {
    locked = false;
    fs::remove(fs::path((getImgRoot()+"/locked").c_str()));
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
        cerr << "Libbe is currently not integrated. Cannot upgrade image inplace. must have New root specified. Use -NS";
        throw std::exception();
    }
    //TODO add correct IMAGE_ROOT_PATH to newRoot Path if we are a full image.
    newRoot += IMAGE_ROOT_PATH;

    if(!fs::is_directory(fs::system_complete(newRoot))){
        fs::create_directories(fs::system_complete(newRoot));
    }
    history.upgrade_format(newRoot);
    config.upgrade_format(newRoot);
    installed.upgrade_format(newRoot);
    known.upgrade_format(newRoot);
    upgrade_needed = false;
}

bool pkg::Image::needsUpgrade() {
    return upgrade_needed;
}

pkg::ImagePlan pkg::Image::makePlan(const std::vector<std::string> &packages) {
    std::vector<pkg::PackageInfo> resolved = known.getPackages(packages);
    pkg::ImagePlan plan(getImgRoot(), getWriteCachePath(), config);
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






























