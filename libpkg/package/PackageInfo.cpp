//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "PackageInfo.h"
#include "PackageException.h"
#include <algorithm>
#include <action/DirectoryAction.h>
#include <vectoradd.h>
#include <action/ActionException.h>
#include <sstream>
#include <iomanip>

using namespace rapidjson;

void pkg::PackageInfo::addAction(const std::string &action_string) {
    try {
        if (boost::algorithm::starts_with(action_string, "set")) {
            AttributeAction act = AttributeAction(action_string);
            if (act.name == "pkg.fmri") {
                this->setFmri(act.values[0]);
            } else if (act.name == "info.classification") {
                this->classification = act.values;
            } else if (act.name == "pkg.summary") {
                this->summary = act.values[0];
            } else if (act.name == "pkg.description") {
                this->description = act.values[0];
            } else if (act.name == "pkg.obsolete") {
                if(act.values[0] == "true"){
                    markObsolete();
                }
            } else if (act.name == "pkg.renamed") {
                if(act.values[0] == "true"){
                    markRenamed();
                }
            } else if (act.name == "pkg.human-version") {
                this->humanversion = act.values[0];
            } else {
                attrs.push_back(act);
            }
        } else if (boost::algorithm::starts_with(action_string, "depend")) {
            dependencies.push_back(DependAction(action_string));
        } else if (boost::algorithm::starts_with(action_string, "dir")) {
            dirs.push_back(DirectoryAction(action_string));
        } else if (boost::algorithm::starts_with(action_string, "file")) {
            files.push_back(FileAction(action_string));
        } else if (boost::algorithm::starts_with(action_string, "link")) {

        } else if (boost::algorithm::starts_with(action_string, "hardlink")) {

        } else if (boost::algorithm::starts_with(action_string, "driver")) {

        } else if (boost::algorithm::starts_with(action_string, "license")) {

        } else if (boost::algorithm::starts_with(action_string, "legacy")) {

        } else if (boost::algorithm::starts_with(action_string, "signature")) {

        } else if (boost::algorithm::starts_with(action_string, "user")) {

        } else if (boost::algorithm::starts_with(action_string, "group")) {

        } else {
            throw pkg::exception::InvalidPackageException(this->name,
                                                          "action_string " + action_string + " not recognized");
        }
    } catch (pkg::action::exception::InvalidActionException actex) {
            throw pkg::exception::InvalidPackageException(this->name, "Error while parsing action: " + std::string(actex.what()));
    }
}

pkg::PackageInfo pkg::PackageInfo::operator+=(const pkg::PackageInfo &alternate) {
    if(this->getFmri() == alternate.getFmri()) {
        this->states += alternate.states;
        this->categories += alternate.categories;
        this->attrs += alternate.attrs;
        this->classification += alternate.classification;
        this->dirs += alternate.dirs;
        this->dependencies += alternate.dependencies;
        this->files += alternate.files;
        this->licenses += alternate.licenses;
        this->links += alternate.links;

        if (!alternate.description.empty()) {
            this->description = alternate.description;
        }
        if (!alternate.summary.empty()) {
            this->summary = alternate.summary;
        }
        if (!alternate.humanversion.empty()) {
            this->humanversion = alternate.humanversion;
        }
        this->renamed = alternate.renamed;
        this->obsolete = alternate.obsolete;
        this->version = alternate.version;
        this->build_release = alternate.build_release;
        this->branch = alternate.branch;
        this->packaging_date = alternate.packaging_date;
    }
    return *this;
}

void pkg::PackageInfo::setFmri(const std::string &fmri) {
    //We must have full package fmri
    if(!boost::starts_with(fmri, "pkg://")) {
        throw pkg::exception::InvalidFMRIException(fmri);
    }
    map<string,string> mapFMRI = PackageInfo::splitFMRI(fmri);
    publisher = mapFMRI["publisher"];
    name = mapFMRI["name"];
    version = mapFMRI["version"];
    build_release = mapFMRI["build_release"];
    branch = mapFMRI["branch"];
    setPackagingDate(mapFMRI["packaging_date"]);
}

void pkg::PackageInfo::markObsolete() {
    this->obsolete = true;
    this->summary.clear();
    this->dependencies.clear();
    this->files.clear();
}

void pkg::PackageInfo::markRenamed() {
    this->renamed = true;
    this->summary.clear();
}

void pkg::PackageInfo::commitManifest(istream &manifest) {
    for (std::string line; std::getline(manifest, line);){
        addAction(line);
    }
    //TODO Fill attributes from actions like csize size etc.
}

bool pkg::PackageInfo::operator==(pkg::PackageInfo &alternate) {
    if(this->getFmri() != alternate.getFmri()) return false;
    if(this->description != alternate.description) return false;
    if(this->summary != alternate.summary) return false;
    for(auto attr : this->attrs){
        if(!alternate.hasAttr(attr)) return false;
    }
    for(auto dep : this->dependencies){
        if(!alternate.hasDependency(dep)) return false;
    }
    //TODO as soon as we have more actions add them here aswell.
    return true;
}

bool pkg::PackageInfo::hasAttr(const AttributeAction &alternate, const bool &exact) {
    for(auto attr : attrs){
        if(exact){
            if (attr.name == alternate.name){
                for(auto value : alternate.values){
                    if(!attr.hasValue(value)) return false;
                }
                return true;
            }
        } else {
            if (attr.name == alternate.name) return true;
        }
    }
    return false;
}

bool pkg::PackageInfo::hasDependency(const DependAction &alternate) {
    for(auto dep : dependencies){
        if(dep.fmri == alternate.fmri && dep.type == alternate.type) return true;
    }
    return false;
}

map<string, string> pkg::PackageInfo::splitFMRI(string FMRI) {
    map<string, string> mapFMRI;
    if(boost::starts_with(FMRI, "pkg://")){
        boost::erase_first(FMRI, "pkg://");
        mapFMRI["publisher"] = FMRI.substr(0, FMRI.find("/"));
        FMRI.erase(0, FMRI.find("/")+1);
    } else {
        boost::erase_first(FMRI, "pkg:/");
    }
    mapFMRI["name"] = FMRI.substr(0, FMRI.find("@"));
    FMRI.erase(0, FMRI.find("@")+1);
    mapFMRI["version"] = FMRI.substr(0, FMRI.find(","));
    FMRI.erase(0, FMRI.find(",")+1);
    mapFMRI["build_release"] = FMRI.substr(0, FMRI.find("-"));
    FMRI.erase(0, FMRI.find("-")+1);
    mapFMRI["branch"] = FMRI.substr(0, FMRI.find(":"));
    FMRI.erase(0, FMRI.find(":")+1);
    mapFMRI["packaging_date"] = FMRI;
    return mapFMRI;
}

//TODO Tests
bool pkg::PackageInfo::component_smaller_than(const string &version1, const string version2) {
    int major1, major2, minor1, minor2, revision1, revision2;
    char revChar1, revChar2;
    if(sscanf(version1.c_str(), "%d.%d.%d%c", &major1, &minor1, &revision1, &revChar1) > 3){
        //If you ask why look at openSSL
        revision1 += revChar1;
    }
    if(sscanf(version2.c_str(), "%d.%d.%d%c", &major2, &minor2, &revision2, &revChar2) > 3){
        revision2 += revChar2;
    }
    if(major1 < major2) return true;
    if(minor1 < minor2) return true;
    return revision1 < revision1;
}

bool pkg::PackageInfo::smaller_than(const string &component_version, const string &alternate_component_version,
                                    tm packaging_date, tm alternate_packaging_date) {
    if(PackageInfo::component_smaller_than(component_version, alternate_component_version)){
        return true;
    }
    return mktime(&packaging_date) < mktime(&alternate_packaging_date);
}

bool pkg::PackageInfo::operator<(const pkg::PackageInfo &alternate) {
    return PackageInfo::smaller_than(version, alternate.version, packaging_date, alternate.packaging_date);
}





