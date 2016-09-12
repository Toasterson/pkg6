//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_ATTRIBUTEACTION_H
#define PKG6_ATTRIBUTEACTION_H


#include <rapidjson/rapidjson.h>
#include <string>
#include <vector>
#include <rapidjson/document.h>
#include <map>
#include "Action.h"

using namespace rapidjson;

namespace pkg{
    namespace action {
        class AttributeAction: public Action{
        public:
            AttributeAction(): Action("set"){}
            explicit AttributeAction(const std::string &action_string): Action("set") {
                parseActionString(action_string);
            }

            std::string name;
            std::vector<std::string> values;
            std::map<std::string,std::string> optionals;
            void parseActionString(std::string action_string);

            std::string toActionString();

            void install(){}

            bool validate(){ return true; }

            template <typename Writer>
            void Serialize(Writer& writer) const{
                if(!name.empty()) {
                    writer.StartObject();
                    writer.String(name.c_str());
                    writer.StartArray();
                    for (auto value : values) {
                        writer.String(value.c_str());
                    }
                    if (!optionals.empty()) {
                        writer.String("opt");
                        writer.StartObject();
                        for (auto pair: optionals) {
                            writer.String(pair.first.c_str());
                            writer.String(pair.second.c_str());
                        }
                        writer.EndObject();
                    }
                    writer.EndArray();
                    writer.EndObject();
                }
            }

            void Deserialize(const Value& rootValue){
                if(rootValue.IsObject()){
                    for(auto itr = rootValue.MemberBegin(); itr != rootValue.MemberEnd(); ++itr){
                        if(itr->name.GetString() == "opt"){
                            for(auto itr2 = itr->value.MemberBegin(); itr2 != itr->value.MemberEnd(); ++itr2){
                                optionals.insert(std::pair<std::string,std::string>(itr2->name.GetString(), itr2->value.GetString()));
                            }
                        } else {
                            name = itr->name.GetString();
                            if (itr->value.IsArray()) {
                                for (rapidjson::SizeType i = 0; i < itr->value.Size(); i++) {
                                    values.push_back(itr->value[i].GetString());
                                }
                            }
                        }
                    }
                }
            }
        };
    }
}

#endif //PKG6_ATTRIBUTEACTION_H
