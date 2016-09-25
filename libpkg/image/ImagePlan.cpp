//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "ImagePlan.h"
#include <http_client/HttpClient.h>
#include <boost/algorithm/string.hpp>

using namespace std;

void pkg::ImagePlan::download() {
    getPackageMetadata();
    //TODO Progress reporting with Megabytes Downloaded
    for(auto pkg: packages) {
        vector<string> origins = config.getPublisher(pkg.publisher).getOrigins();
        //TODO Check if we want multithreading here
        for (auto origin: origins) {
            //Make http request to first host that works
            try {
                std::vector<std::string> splited;
                boost::split(splited, origin, boost::is_any_of("/"));
                boost::erase_all(splited[0], ":");
                HttpClient client(splited[0], splited[1], splited[2], pkg.publisher);
                std::string cacheDir = CACHE_ROOT+"/"+pkg.getFmri();
                //Get all the files for installation
                for(auto file : pkg.files){
                    std::ofstream filestream(cacheDir+"/"+file.name);
                    client.getFile(file.name, filestream);
                    //TODO Check for file corruption
                }
                //If we have the file skip getting it from other hosts
                continue;
            } catch (...){
                //TODO find out how to catch http exception
            }
        }
    }
}

void pkg::ImagePlan::install() {
    download();
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

void pkg::ImagePlan::add(const std::vector<pkg::PackageInfo> &pkgs) {
    for(auto pkg: pkgs){
        add(pkg);
    }
}

void pkg::ImagePlan::getPackageMetadata() {
    Progress progDownload("Downloading Manifests", "", (const int &) packages.size());
    for (auto pkg: packages) {
        for (auto origin: config.getPublisher(pkg.publisher).getOrigins()) {
            //Make http request to first host that works
            try {
                std::vector<std::string> splited;
                boost::split(splited, origin, boost::is_any_of("/"));
                boost::erase_all(splited[0], ":");
                HttpClient client(splited[0], splited[1], splited[2], pkg.publisher);
                std::string cacheDir = CACHE_ROOT + "/" + pkg.getFmri();
                if (!boost::filesystem::is_directory(cacheDir)) {
                    boost::filesystem::create_directories(cacheDir);
                }

                if (!boost::filesystem::exists(cacheDir + "/manifest")) {
                    std::ofstream manifest(cacheDir + "/manifest");
                    client.getManifest(pkg.getFmri(), manifest);
                }
                progDownload++;
                //If we have the file skip getting it from other hosts
                continue;
            } catch (...) {
                //TODO find out how to catch http exception
            }
        }
    }
    Progress progCommit("Commiting Manifests", "", (const int &) packages.size());
    for(auto pkg:packages) {
        ifstream manifest(CACHE_ROOT + "/" + pkg.getFmri() + "/manifest");
        pkg.commitManifest(manifest);
        progCommit++;
    }
}

