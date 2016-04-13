//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "Image.h"

std::string pkg::Image::getImgDir(){
    return "";
}

std::string pkg::Image::getImgRoot() {
    return "";
}


bool pkg::Image::isLocked() {
    return locked;
}

std::string pkg::Image::getWriteCachePath() {
    return "";
}

std::string pkg::Image::getArch() {
    //return config.getVariant("variant.arch");
    return "";
}

pkg::Image::Image(const std::string &root, const bool &allow_ondisk_upgrade):
    history(pkg::history::History(root)),
    config(pkg::ImageConfig(root)),
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

void pkg::Image::upgrade_format(const bool &allow_unprevileged) {

}

void pkg::Image::create(const bool &is_zone, const std::vector<std::string> &publishers, const bool &refresh_allowed,
                        const std::map<std::string, std::string> &properties,
                        const std::map<std::string, std::string> &variants,
                        const std::map<std::string, std::string> &facets) {

}

std::tm pkg::Image::getLastModified() {
    return tm();
}


























