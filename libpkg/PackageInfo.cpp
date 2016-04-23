//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "PackageInfo.h"
#include <string>

void pkg::PackageInfo::addAttr(const std::string &set_string) {
    std::string set_delim = "set ";
    if(set_string.find(set_delim) != std::string::npos){
        std::string s = set_string;
        size_t pos = 0;
        std::string token;
        std::string delimiter = "=";
        std::string key, value;
        s.erase(0, s.find(set_delim) + set_delim.length());
        //TODO Proper set action parser. This does not work.
        while ((pos = s.find(delimiter)) != std::string::npos) {
            token = s.substr(0, pos);
            if(token == "name"){
                s.erase(0, 4 + delimiter.length());
                size_t secondpos = s.find(" ");
                key = s.substr(0, secondpos);
                s.erase(0, secondpos +1);
            } if(token == "value"){
                s.erase(0, 5 + delimiter.length());
                size_t secondpos = s.find(" ");
                value = s.substr(0, secondpos);
                s.erase(0, secondpos +1);
            }
        }
        attrs.put(key, value);
    }
}

