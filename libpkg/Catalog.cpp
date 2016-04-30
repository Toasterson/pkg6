//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <pkgdefs.h>
#include "Catalog.h"
#include <archive.h>
#include <Progress.h>
#define BOOST_SYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/info_parser.hpp>

namespace fs = boost::filesystem;
using namespace boost::property_tree;

void pkg::Catalog::upgrade_format(const std::string &newRoot) {
    std::string importDir = root_dir;
    root_dir = newRoot;
    read_only = false;
    importpkg5(importDir);
}

pkg::Catalog::Catalog(const std::string &root, const std::string &name, const bool &read_only, const bool &do_sign):
    root_dir(root),
    name(name),
    read_only(read_only),
    do_sign(do_sign)
{
    //If root_dir contains pkg5 metadata make catalog readonly thus requiring upgrade_format is in pkg5 format import into memory
    // As the Catlog in pkg6 will always reside on disk a load will not be required
    if(fs::is_regular_file(fs::system_complete((root_dir+"/catalog.attrs").c_str()))){
        this->read_only = true;
    }
}

void pkg::Catalog::importpkg5(const std::string& importDir) {

    ptree catalog_attrs, catalog_base, catalog_dependency, catalog_summary;
    read_json(importDir + "/catalog.attrs", catalog_attrs);
    read_json(importDir + "/catalog.base.C", catalog_base);
    read_json(importDir + "/catalog.dependency.C", catalog_dependency);
    read_json(importDir + "/catalog.summary.C", catalog_summary);

    Progress progress = Progress("importing pkg5 metadata", "packages", catalog_attrs.get<int>("package-version-count"));

    for (auto publisher : catalog_base){
        std::string publisher_name = publisher.first;

        for(auto package : publisher.second){
            std::string package_name = package.first;

            for(auto package_version : package.second){
                std::string base_path = publisher_name+"\\"+package_name;

                pkg::PackageInfo pkg = pkg::PackageInfo();
                pkg.publisher = publisher_name;
                pkg.name = package_name;
                pkg.version = package_version.second.get<std::string>("version");
                pkg.signature = package_version.second.get<std::string>("signature-sha-1");

                for (auto package_state : package_version.second.get_child("metadata.states")){
                    pkg.states.push_back(package_state.second.get<int>(""));
                }

                //TODO dependency parser

                /*
                boost::optional<ptree&> check_package_dependency = catalog_dependency.get_child_optional(ptree::path_type(base_path, '\\'));
                if(check_package_dependency) {
                    ptree package_dependency = catalog_dependency.get_child(ptree::path_type(base_path, '\\'));
                    for (auto package_dependency_version: package_dependency) {
                        int dependency_count = 1;
                        if (package_version.second.get<std::string>("version") ==
                            package_dependency_version.second.get<std::string>("version")) {
                            for (auto package_dependency_action: package_dependency_version.second.get_child("actions")) {
                                //data.put(ptree::path_type(data_path+"\\dependencies\\"+std::to_string(dependency_count), '\\'), package_dependency_action.second.get<std::string>(""));
                                dependency_count++;
                            }
                            dependency_count = 1;
                        }
                    }
                }
                */

                boost::optional<ptree&> check_package_summary = catalog_summary.get_child_optional(ptree::path_type(base_path, '\\'));
                if(check_package_summary) {
                    ptree package_summary = catalog_summary.get_child(ptree::path_type(base_path, '\\'));
                    for (auto package_summary_version: package_summary) {
                        if (package_version.second.get<std::string>("version") ==
                                package_summary_version.second.get<std::string>("version")) {
                            for (auto package_summary_action: package_summary_version.second.get_child("actions")) {
                                pkg.addAction(package_summary_action.second.get<std::string>(""));
                            }
                        }
                    }
                }
                addPackage(pkg);
                progress++;
            }
        }
    }
}

void pkg::Catalog::addPackage(pkg::PackageInfo &pkg) {
    //Write Directory Structure of Package if does not exist
    std::string pkg_path_str = (path()+"/"+pkg.publisher+"/"+pkg.name+"/"+pkg.version);
    fs::path pkg_path = fs::system_complete(pkg_path_str.c_str());
    if(!fs::is_directory(pkg_path)){
        fs::create_directories(fs::system_complete(pkg_path));
    }
    //write attrs file
    std::ofstream attrStream = std::ofstream(pkg_path_str + "/attrs");
    for(auto attr: pkg.attrs) {
        write_info(attrStream, attr.data);
    }

    //Write dependency file
    std::ofstream depStream = std::ofstream(pkg_path_str + "/dep");
    for(auto dep: pkg.dependencies) {
        write_info(depStream, dep.data);
    }
}

void pkg::Catalog::removePackage(const pkg::PackageInfo &pkg) {

}

pkg::PackageInfo pkg::Catalog::getPackage(const std::string &name) {
    return pkg::PackageInfo();
}

std::string pkg::Catalog::path() {
    return root_dir + "/" + name;
}
