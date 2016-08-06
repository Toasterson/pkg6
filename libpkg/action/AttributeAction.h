//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_ATTRIBUTEACTION_H
#define PKG6_ATTRIBUTEACTION_H


#include <rapidjson.h>
#include <string>
#include <vector>
#include <document.h>

using namespace rapidjson;

namespace pkg{
    namespace action {
        class AttributeAction{
        public:
            AttributeAction(): action_type("set"){}
            AttributeAction(const std::string &action_string): action_type("set") {
                parseActionString(action_string);
            }

            std::string action_type;
            std::string name;
            std::vector<std::string> values;
            void parseActionString(std::string action_string);

            std::string toActionString();

            template <typename Writer>
            void Serialize(Writer& writer) const{
                writer.StartObject();
                writer.String(name.c_str());
                writer.StartArray();
                for(auto value : values) {
                    writer.String(value.c_str());
                }
                writer.EndArray();
                writer.EndObject();
            }

            void Deserialize(const Value& rootValue){
                if(rootValue.IsObject()){
                    for(Value::ConstMemberIterator itr = rootValue.MemberBegin(); itr == rootValue.MemberEnd(); ++itr){
                        name = itr->name.GetString();
                        if(itr->value.IsArray()) {
                            for (rapidjson::SizeType i = 0; i < itr->value.Size(); i++) {
                                values.push_back(itr->value[i].GetString());
                            }
                        }
                    }
                }
            }
        };
    }
}

#endif //PKG6_ATTRIBUTEACTION_H
