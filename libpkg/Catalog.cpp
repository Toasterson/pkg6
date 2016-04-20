//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include <pkgdefs.h>
#include "Catalog.h"
#define BOOST_SYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
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
    boost::property_tree::ptree catalog_attrs, catalog_base, catalog_dependency, catalog_summary;
    boost::property_tree::read_json(root_dir+"/catalog.attrs", catalog_attrs);
    boost::property_tree::read_json(root_dir+"/catalog.base.C", catalog_base);
    boost::property_tree::read_json(root_dir+"/catalog.dependency.C", catalog_dependency);
    boost::property_tree::read_json(root_dir+"/catalog.summary.C", catalog_summary);

    data.put("package-count", catalog_attrs.get<std::string>("package-count"));
    data.put("version", 2);
    boost::property_tree::ptree::iterator end = catalog_base.end();
    for (boost::property_tree::ptree::iterator publisher = catalog_base.begin(); publisher != end; ++publisher){
        std::string publisher_name = publisher->first;
        boost::property_tree::ptree::iterator pub_end = publisher->second.end();
        for(boost::property_tree::ptree::iterator package = publisher->second.begin(); package != pub_end; ++package){
            std::string package_name = package->first;
            boost::property_tree::ptree::iterator package_end = package->second.end();
            for(boost::property_tree::ptree::iterator package_version = package->second.begin(); package_version != package_end; ++package_version){
                std::string base_path = publisher_name+"\\"+package_name;
                std::string data_path = base_path+"\\"+package_version->second.get<std::string>("version");
                data.put(boost::property_tree::ptree::path_type(data_path+"\\signature-sha-1", '\\'), package_version->second.get<std::string>("signature-sha-1"));
            }
        }
    }
}

void pkg::Catalog::save() {
    boost::property_tree::write_info(root_dir+"/"+GENERIC_CATALOG_FILENAME, data);
}

void pkg::Catalog::load() {

}









