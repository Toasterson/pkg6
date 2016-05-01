//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "Image.h"

std::string pkg::Image::getImgDir(){
    return "";
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
    history(pkg::history::History(root)),
    config(pkg::ImageConfig(root)),
    installed(pkg::Catalog(root, "installed")),
    //known(pkg::Catalog(root, "known")),
    allow_ondisk_upgrade(allow_ondisk_upgrade)
{

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
        newRoot = IMAGE_ROOT_PATH;
    }
    history.upgrade_format(newRoot);
    config.upgrade_format(newRoot);
    installed.upgrade_format(newRoot);
    //known.upgrade_format(newRoot);
}

void pkg::Image::importpkg5() {

}






























