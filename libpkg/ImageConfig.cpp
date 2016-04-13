//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "ImageConfig.h"

pkg::ImageConfig::ImageConfig(const std::string &root):
    root(root)
{
    //load();
}

/*void pkg::ImageConfig::load() {
    //ini.LoadFile((root + "/" + IMAGE_CONFIG_FILENAME).c_str());
    //TODO Load Publishers into own map
}

void pkg::ImageConfig::save() {
    //ini.SaveFile((root + "/" + IMAGE_CONFIG_FILENAME).c_str());
}

std::string pkg::ImageConfig::getVariant(std::string name) {
    return ini.GetValue("variant", name.c_str(), "");
}

std::string pkg::ImageConfig::getFacet(std::string name) {
    return ini.GetValue("facet", name.c_str(), "");
}

std::string pkg::ImageConfig::getPublisher(std::string name) {
    return "";
}

std::string pkg::ImageConfig::getMediator(std::string name) {
    return ini.GetValue("mediators", name.c_str(), "");
}

std::string pkg::ImageConfig::getImageProperty(std::string name) {
    return ini.GetValue("property", name.c_str(), "");
}

std::vector<std::string> pkg::ImageConfig::getSectionVariant(std::vector<std::string> &filter) {
    return std::vector<std::__cxx11::string>();
}

std::vector<std::string> pkg::ImageConfig::getSectionFacet(std::vector<std::string> &filter) {
    return std::vector<std::__cxx11::string>();
}

std::vector<std::string> pkg::ImageConfig::getSectionPublisher(std::vector<std::string> &filter) {
    return std::vector<std::__cxx11::string>();
}

std::vector<std::string> pkg::ImageConfig::getSectionMediator(std::vector<std::string> &filter) {
    return std::vector<std::__cxx11::string>();
}

std::vector<std::string> pkg::ImageConfig::getSectionImage(std::vector<std::string> &filter) {
    return std::vector<std::__cxx11::string>();
}*/

void pkg::ImageConfig::upgrade_format() {

}

































