//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_DEPENDACTION_H
#define PKG6_DEPENDACTION_H

#include <document.h>
#include <map>
#include <string>
#include "Action.h"

using namespace rapidjson;

namespace pkg{
    namespace action{
        class DependAction: Action{
        public:
            DependAction(): action_type("depend"){}
            DependAction(const std::string &action_string): action_type("depend") {
                    parseActionString(action_string);
            }

            std::string action_type;
            std::string fmri;
            std::string type;
            std::string predicate;
            std::map<std::string,std::string> optional;
            void parseActionString(std::string action_string);

            std::string toActionString();

            void install(){}

            bool validate(){ return true; }

            template <typename Writer>
            void Serialize(Writer& writer) const{
                writer.StartObject();
                writer.String("fmri");
                writer.String(fmri.c_str());
                writer.String("type");
                writer.String(type.c_str());
                if(!predicate.empty()){
                    writer.String("predicate");
                    writer.String(predicate.c_str());
                }
                if(!optional.empty()){
                    for(std::pair<std::string,std::string> opt : optional){
                        writer.String(opt.first.c_str());
                        writer.String(opt.second.c_str());
                    }
                }
                writer.EndObject();
            }

            void Deserialize(const Value& rootValue){
                if(rootValue.IsObject()){
                    for(Value::ConstMemberIterator itr = rootValue.MemberBegin(); itr == rootValue.MemberEnd(); ++itr){
                        if(itr->name.GetString() == "fmri"){
                            fmri = itr->value.GetString();
                        } else if(itr->name.GetString() == "type"){
                            type = itr->value.GetString();
                        } else if(itr->name.GetString() == "predicate"){
                            predicate = itr->value.GetString();
                        } else {
                            optional.insert(std::pair<std::string,std::string>(itr->name.GetString(), itr->value.GetString()));
                        }
                    }
                }
            }
        };
    }
}



#endif //PKG6_DEPENDACTION_H
