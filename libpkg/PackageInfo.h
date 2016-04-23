//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_PACKAGEINFO_H
#define PKG6_PACKAGEINFO_H

#include <string>
#include <vector>
#include <ctime>
#include <boost/property_tree/ptree.hpp>
#include "PackageCategory.h"
#include "LicenseInfo.h"

using namespace boost::property_tree;

namespace pkg {
/*
 * A class capturing the information about packages that a client
 * could need. The fmri is guaranteed to be set. All other values may
 * be None, depending on how the PackageInfo instance was created.
 */
    class PackageInfo {
        /*
         Possible package states; these constants should match the values used
         by the Image class.  Constants with negative values are not currently
         available.
         */
        const int INCORPORATED = -2;
        const int EXCLUDES = -3;
        //For other States see pkgdefs.h
        //TODO Check if these two constants need to be moved to pkgdefs.h
    public:
        std::vector<pkg::PackageCategory> categories;
        std::vector<int> states;
        std::string publisher;
        std::string version;
        std::string build_release;
        std::string branch;
        std::tm packaging_date;
        //TODO check what type these could be
        std::string size;
        std::string csize;
        ////
        std::string name;
        std::vector<LicenseInfo> licenses;
        std::tm last_update;
        std::tm last_install;

        ptree attrs, links, files, dirs, dependencies;

        std::string getDescription(){
            return attrs.get<std::string>("description");
        }

        void setDescription(const std::string& desc){
            attrs.put("description", desc);
        }

        std::string getSummary(){
            return attrs.get<std::string>("summary");
        }

        void setSummary(const std::string& sum){
            attrs.put("summary", sum);
        }

        void setSignature(const std::string& sig){
            attrs.put("signature-sha-1", sig);
        }

        std::string getSignature(){
            return attrs.get<std::string>("signature-sha-1");
        }

        void addAttr(const std::string& set_string);

        std::string getFmri(){
            return publisher + "/" + name + "/" + version;
        }

    };

};
#endif //PKG6_PACKAGEINFO_H
