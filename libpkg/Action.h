//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_ACTION_H
#define PKG6_ACTION_H

#include <string>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <document.h>
#include <map>
#include <writer.h>

using namespace rapidjson;

namespace pkg {
    class Action {
    public:
        Action(){}
        Action(const std::string& action_string){
            parseActionString(action_string);
        }

        void parseActionString(const std::string &action_string);

        std::string toActionString();

        std::string type;

        std::map<std::string,std::string> data;

        template <typename Writer>
        void Serialize(Writer& writer) const{
            writer.StartObject();
            for(auto item : data){
                writer.String(item.first.c_str());
                writer.String(item.second.c_str());
            }
            writer.EndObject();
        }

        void Deserialize(const Value& rootVal, const std::string& type){
            this->type = type;
            if(rootVal.IsObject()){
                for (Value::ConstMemberIterator itr = rootVal.MemberBegin(); itr != rootVal.MemberEnd(); ++itr){
                    data[itr->name.GetString()] = itr->value.GetString();
                }
            }
        }

    };
};
#endif //PKG6_ACTION_H
