//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <pkgdefs.h>
#include "Catalog.h"
#define BOOST_SYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/info_parser.hpp>

namespace fs = boost::filesystem;


void pkg::Catalog::upgrade_format(const std::string &newRoot) {

}

pkg::Catalog::Catalog(const std::string &root, const std::string &catalogtype, const bool &read_only, const bool &do_sign):
    root_dir(root),
    catalogtype(catalogtype),
    read_only(read_only),
    do_sign(do_sign)
{
    //If state/installed is in pkg5 format import into memory
    //if(fs::is_regular_file(fs::system_complete((root_dir+"/catalog.attrs").c_str()))){
        importpkg5();
    //} else{
    //    load();
    //}
}

void pkg::Catalog::importpkg5() {
    using namespace boost::property_tree;

    ptree catalog_attrs, catalog_base, catalog_dependency, catalog_summary;
    read_json(root_dir+"/catalog.attrs", catalog_attrs);
    read_json(root_dir+"/catalog.base.C", catalog_base);
    read_json(root_dir+"/catalog.dependency.C", catalog_dependency);
    read_json(root_dir+"/catalog.summary.C", catalog_summary);

    data.put("package-count", catalog_attrs.get<std::string>("package-count"));
    data.put("version", 2);

    for (auto publisher : catalog_base){
        std::string publisher_name = publisher.first;

        for(auto package : publisher.second){
            std::string package_name = package.first;

            for(auto package_version : package.second){
                std::string base_path = publisher_name+"\\"+package_name;
                std::string data_path = base_path+"\\"+package_version.second.get<std::string>("version");
                data.put(ptree::path_type(data_path+"\\signature-sha-1", '\\'), package_version.second.get<std::string>("signature-sha-1"));

                ptree state_tree = ptree();
                for (auto package_state : package_version.second.get_child("metadata.states")){
                    state_tree.put(package_state.second.get<std::string>(""), true);
                }
                data.put_child(ptree::path_type(data_path+"\\states", '\\'), state_tree);

                //TODO maybe i want the dependencies listed differently in my catalog version need to decide


                boost::optional<ptree&> check_package_dependency = catalog_dependency.get_child_optional(ptree::path_type(base_path, '\\'));
                if(check_package_dependency) {
                    ptree package_dependency = catalog_dependency.get_child(ptree::path_type(base_path, '\\'));
                    for (auto package_dependency_version: package_dependency) {
                        int dependency_count = 1;
                        if (package_version.second.get<std::string>("version") ==
                            package_dependency_version.second.get<std::string>("version")) {
                            for (auto package_dependency_action: package_dependency_version.second.get_child("actions")) {
                                data.put(ptree::path_type(data_path+"\\dependencies\\"+std::to_string(dependency_count), '\\'), package_dependency_action.second.get<std::string>(""));
                                dependency_count++;
                            }
                            dependency_count = 1;
                        }
                    }
                }

                boost::optional<ptree&> check_package_summary = catalog_summary.get_child_optional(ptree::path_type(base_path, '\\'));
                if(check_package_summary) {
                    ptree package_summary = catalog_summary.get_child(ptree::path_type(base_path, '\\'));
                    for (auto package_summary_version: package_summary) {
                        int action_count = 1;
                        if (package_version.second.get<std::string>("version") ==
                                package_summary_version.second.get<std::string>("version")) {
                            for (auto package_summary_action: package_summary_version.second.get_child("actions")) {
                                data.put(ptree::path_type(data_path+"\\actions\\"+std::to_string(action_count), '\\'), package_summary_action.second.get<std::string>(""));
                                action_count++;
                            }
                            action_count = 1;
                        }
                    }
                }
            }
        }
    }
}

void pkg::Catalog::save() {
    boost::property_tree::write_info(root_dir+"/"+GENERIC_CATALOG_FILENAME, data);
}

void pkg::Catalog::load() {

}









