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
    private:
        //const std::string action_types[5] = {"set", "depend", "dir", "file", "license", "link"};
    public:
        Action(){}
        Action(const std::string& action_string){
            parseActionString(action_string);
        }

        void parseActionString(const std::string &action_string);

        std::string toActionString();

        std::string type;

        std::vector<std::string> data;

        template <typename Writer>
        void Serialize(Writer& writer) const{
            writer.StartObject();
            writer.String("type");
            writer.String(type.c_str());
            writer.String("data");
            writer.StartArray();
            for(std::string val : data){
                writer.String(val.c_str());
            }
            writer.EndArray();
            writer.EndObject();
        }

        void Deserialize(const Value& rootVal){
            if(rootVal.IsObject()){
                this->type = rootVal["type"].GetString();
                const Value& value = rootVal["data"];
                for (Value::ConstValueIterator itr = value.Begin(); itr != value.End(); ++itr){
                    this->data.push_back(itr->GetString());
                }
            }
        }

    };
};
#endif //PKG6_ACTION_H
