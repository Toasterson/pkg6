//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <vector>
#include "ImageConfig.h"
#include "ImageException.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp>

using namespace std;

pkg::ImageConfig::ImageConfig(const std::string &root):
    IMAGE_ROOT(root)
{
    if(boost::filesystem::is_regular_file(IMAGE_ROOT+"/"+IMAGE_CONFIG_OLDFILENAME)){
        std::ifstream ifstream1(IMAGE_ROOT+"/"+IMAGE_CONFIG_OLDFILENAME);
        importpkg5(ifstream1);
    } else {
        load(std::ifstream(IMAGE_ROOT+"/"+IMAGE_CONFIG_FILENAME));
    }
}


void pkg::ImageConfig::importpkg5(istream& oldconfig) {
    string section;
    for (std::string line; std::getline(oldconfig, line); )
    {
        std::istringstream iss(line);
        std::string id, val;

        if (!std::getline(iss, id, '=')) {
            throw exception::ImageConfigException(line);
        } else if (id[0] == '#') {
            continue;
        } else if(id[0] == '['){
            id.erase(0, 1);
            id.erase(id.size()-1, id.size());
            section = id;
            continue;
        } else if (!std::getline(iss, val, '=')) {
            continue;
        }
        //We should come to this position in the loop when we have no section parsed.
        if(section.empty()){
            throw exception::ImageConfigException(line);
        }

        //We have the Publisher section so parse that
        if(boost::starts_with(section, "authority_")){
            string publisher = section;
            publisher.erase(0, publisher.find("_")+1);
            if(publishers.count(publisher) == 0){
                Publisher pub;
                publishers.insert(pair<string, Publisher>(publisher, pub));
            }
            if(id == "disabled"){
                publishers[publisher].setGeneralProperty("disabled", val);
            } else if(id == "sticky"){
                publishers[publisher].setGeneralProperty("sticky", val);
            } else if(id == "uuid"){
                publishers[publisher].setGeneralProperty("uuid", val);
            } else if(id == "origins"){
                val.erase(0, 2);
                val.erase(val.size()-2, val.size());
                publishers[publisher].setGeneralProperty("origins", val);
            } else if(id == "alias"){
                publishers[publisher].setGeneralProperty("alias", val);
            } else if(id == "prefix"){
                publishers[publisher].setGeneralProperty("prefix", val);
            } else if(id == "repo.refresh_seconds"){
                publishers[publisher].setRepoProperty("refresh_seconds", val);
            } else if(id == "repo.registration_uri"){
                publishers[publisher].setRepoProperty("registration_uri", val);
            } else if(id == "repo.registered"){
                publishers[publisher].setRepoProperty("registered", val);
            } else if(id == "repo.name"){
                publishers[publisher].setRepoProperty("name", val);
            } else if(id == "repo.sort_policy"){
                publishers[publisher].setRepoProperty("sort_policy", val);
            }
        } else if(section == "variant"){
            id.erase(0, id.find(".")+1);
            variants[id] = val;
        } else if(section == "property"){
            properties[id] = val;
        }
    }
}

void pkg::ImageConfig::load(const std::istream &config) {

}

void pkg::ImageConfig::save() {
    if(VERSION < 4){
        throw pkg::exception::ImageConfigException("Image must be upgraded first");
    }


}

void pkg::ImageConfig::save(std::ostream &output) {
    if(VERSION < 4){
        throw pkg::exception::ImageConfigException("Image must be upgraded first");
    }

}

void pkg::ImageConfig::upgrade_format(const std::string &newRoot) {

}

std::string pkg::ImageConfig::getVariant(const std::string &name) {
    return variants[name];
}

pkg::Publisher pkg::ImageConfig::getPublisher(const std::string &name) {
    return publishers[name];
}

std::string pkg::ImageConfig::getImageProperty(const std::string &name) {
    return properties[name];
}

pkg::Publisher pkg::ImageConfig::getFirstPublisher() {
    return publishers.begin()->second;
}


/*
 * void pkg::Catalog::savePackage(pkg::PackageInfo &pkg) {
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
 *
 *
 */


































