//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "ImagePlan.h"
#include <http_client/HttpClient.h>
#include <boost/algorithm/string.hpp>

using namespace std;

void pkg::ImagePlan::download() {
    for(auto pkg: packages) {
        vector<string> origins = config.getPublisher(pkg.publisher).getOrigins();
        //TODO This for loop could be Multithreaded
        //TODO Progress reporting
        for (auto origin: origins) {
            std::vector<std::string> splited;
            boost::split(splited, origin, boost::is_any_of("/"));
            boost::erase_all(splited[0], ":");
            HttpClient client(splited[0], splited[1], splited[2], pkg.publisher);
            //Get Manifest from url or local dir if already present
            std::string cacheDir = CACHE_ROOT+"/"+pkg.getFmri();
            if(!boost::filesystem::is_directory(cacheDir)){
                boost::filesystem::create_directories(cacheDir);
            }

            if(!boost::filesystem::exists(cacheDir+"/manifest")) {
                std::ofstream manifest(cacheDir+"/manifest");
                client.getManifest(pkg.getFmri(), manifest);
            }
            //Get all the files for installation
            for(auto file : pkg.files){
                std::ofstream filestream(cacheDir+"/"+file.name);
                client.getFile(file.name, filestream);
            }
            continue;
        }
    }
}

void pkg::ImagePlan::install() {
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

void pkg::ImagePlan::add(const std::vector<pkg::PackageInfo> &pkgs) {
    for(auto pkg: pkgs){
        add(pkg);
    }
}

pkg::ImagePlan::ImagePlan(const std::vector<pkg::PackageInfo> &packages, const pkg::ImageConfig &config, const std::string& cache)
                :config(config),
                 CACHE_ROOT(cache)
{
    add(packages);
}

