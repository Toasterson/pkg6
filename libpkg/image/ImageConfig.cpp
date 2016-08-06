//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "ImageConfig.h"

pkg::ImageConfig::ImageConfig(const std::string &root):
    root(root)
{
    if(boost::filesystem::is_regular_file((root + "/" + IMAGE_CONFIG_OLDFILENAME).c_str())){
        importpkg5();
    } else {
        load();
    }
}

void pkg::ImageConfig::load() {
    boost::property_tree::ini_parser::read_ini((root + "/" + IMAGE_CONFIG_FILENAME).c_str(), pt);
    //TODO Load Publishers into own map
}

void pkg::ImageConfig::save() {
    boost::property_tree::write_ini((root + "/" + IMAGE_CONFIG_FILENAME).c_str(), pt);
}

std::string pkg::ImageConfig::getVariant(std::string name) {
    return pt.get<std::string>(boost::property_tree::ptree::path_type("variant/"+name, '/'));
}

std::string pkg::ImageConfig::getFacet(std::string name) {
    return pt.get<std::string>(boost::property_tree::ptree::path_type("facet/"+name, '/'));
}

std::string pkg::ImageConfig::getPublisher(std::string name) {
    return "";
}

std::string pkg::ImageConfig::getMediator(std::string name) {
    return pt.get<std::string>(boost::property_tree::ptree::path_type("mediators/"+name, '/'));
}

std::string pkg::ImageConfig::getImageProperty(std::string name) {
    return pt.get<std::string>(boost::property_tree::ptree::path_type("property/"+name, '/'));
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
}

void pkg::ImageConfig::upgrade_format(const std::string &newRoot) {
    std::string filename = newRoot + "/" + IMAGE_CONFIG_FILENAME;
    boost::property_tree::ini_parser::write_ini((filename).c_str(), pt);
}

void pkg::ImageConfig::importpkg5() {
    boost::property_tree::ini_parser::read_ini((root + "/" + IMAGE_CONFIG_OLDFILENAME).c_str(), pt);
}



































