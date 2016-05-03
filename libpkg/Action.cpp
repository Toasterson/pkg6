//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "Action.h"

void pkg::Action::parseActionString(const std::string &action_string) {
    boost::tokenizer<boost::escaped_list_separator<char>> tokens(action_string, boost::escaped_list_separator<char>("\\", " ", "\""));
    std::string key = "";
    std::vector<std::string> values = std::vector<std::string>();
    auto iter = std::begin(tokens);
    for(std::string token: tokens){
        if(iter == std::begin(tokens)){
            type = token;
        } else {
            if(boost::algorithm::contains(token, "name=")){
                key = boost::algorithm::erase_first_copy(token, "name=");
            } else if(boost::algorithm::contains(token, "value=")){
                values.push_back(boost::algorithm::erase_first_copy(token, "value="));
            } else {
                values.push_back(token);
            }
        }
        iter++;
    }
    if(key == "") {
        key = type;
    }
    if(values.size() > 1) {
        for (int it = 0; it < values.size(); it++) {
            std::string val = values.at((unsigned long) it);
            if (boost::algorithm::contains(val, "=")) {
                std::vector<std::string> kv = std::vector<std::string>();
                boost::algorithm::split(kv, val, boost::algorithm::is_any_of("="));
                data.put(ptree::path_type((key + "\\" + kv.front()), '\\'), kv.back());
            } else {
                data.put(ptree::path_type(key + "\\" + std::to_string(it), '\\'), val);
            }
        }
    } else {
        std::string val = values.front();
        data.put(ptree::path_type(key, '\\'), val);
    }

}

std::string pkg::Action::toActionString() {
    return "";
}

void pkg::Action::put(const std::string &key, const std::string &value) {
    data.put(ptree::path_type(key, '\\'), value);
}

std::string pkg::Action::get(const std::string &key) {
    return data.get<std::string>(ptree::path_type(key, '\\'));
}

bool pkg::Action::hasKey(const std::string &key) {
    boost::optional<ptree&> optree = data.get_child_optional(ptree::path_type(key, '\\'));
    if(optree){
        return true;
    } else {
        return false;
    }
}

void pkg::Action::update(const std::string &key, const std::string &value) {
    if(hasKey(key)){
        put(key, value);
    }
}













