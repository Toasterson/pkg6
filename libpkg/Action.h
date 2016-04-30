//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_ACTION_H
#define PKG6_ACTION_H

#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

using namespace boost::property_tree;
namespace pkg {
    class Action {
    private:
        //const std::string action_types[5] = {"set", "depend", "dir", "file", "license", "link"};
    public:
        Action(){}
        Action(const std::string& action_string){ parseActionString(action_string); }
        void parseActionString(const std::string &action_string);

        std::string toActionString();

        std::string get(const std::string &key);

        void put(const std::string &key, std::string &value);

        std::string type;

        ptree data;
    };
};
#endif //PKG6_ACTION_H
