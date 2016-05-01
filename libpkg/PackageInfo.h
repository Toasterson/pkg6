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
#include "Action.h"
#include <boost/tokenizer.hpp>

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
        std::string signature;
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



        std::vector<Action> attrs, links, files, dirs, dependencies;

        std::string getDescription(){
            for(Action act: attrs){
                try {
                    return act.get("description");
                }
                catch (int e){

                }
            }
            return "";
        }

        std::string getSummary(){
            for(Action act: attrs){
                try {
                    return act.get("summary");
                }
                catch (int e){

                }
            }
            return "";
        }

        std::string getFmri(){
            return publisher + "/" + name + "/" + version;
        }

        void addAction(const std::string& action_string);

        void addAction(const Action& action);

    };

};
#endif //PKG6_PACKAGEINFO_H
