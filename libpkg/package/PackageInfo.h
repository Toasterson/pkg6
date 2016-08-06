//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_PACKAGEINFO_H
#define PKG6_PACKAGEINFO_H
#define RAPIDJSON_HAS_STDSTRING 1
#include <string>
#include <vector>
#include <ctime>
#include <boost/property_tree/ptree.hpp>
#include "PackageCategory.h"
#include "image/LicenseInfo.h"
#include "action/Action.h"
#include <boost/tokenizer.hpp>
#include <document.h>

using namespace boost::property_tree;
using namespace rapidjson;

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
        static const int INCORPORATED = -2;
        static const int EXCLUDES = -3;
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

        PackageInfo(){}

        PackageInfo(const std::string& publisher, const std::string& name, const std::string& version): publisher(publisher), name(name), version(version){}
        PackageInfo(const PackageInfo& origin): publisher(origin.publisher), name(origin.name), version(origin.version){}
        PackageInfo(const std::string& FMRI){

        }

        std::string getDescription(){
            return "";
        }

        std::string getSummary(){
            return "";
        }

        std::string getFmri(){
            return publisher + "/" + name + "@" + version;
        }

        std::string getFilePath() {
            return getFmri() + ".json";
        }

        void addAction(const std::string& action_string);

        void addAction(const Action& action);

        PackageInfo operator+=(PackageInfo& alternate);

        template <typename Writer>
        void Serialize(Writer& writer) const{
            writer.StartObject();
            writer.String("publisher");
            writer.String(publisher.c_str());
            writer.String("name");
            writer.String(name.c_str());
            writer.String("version");
            writer.String(version.c_str());
            writer.String("signature");
            writer.String(signature.c_str());
            writer.String("states");
            writer.StartArray();
            for(int state : states){
                writer.Int(state);
            }
            writer.EndArray();
            writer.String("attrs");
            writer.StartArray();
            for(Action attr : attrs){
                attr.Serialize(writer);
            }
            writer.EndArray();
            writer.String("dependencies");
            writer.StartArray();
            for(Action dep : dependencies){
                dep.Serialize(writer);
            }
            writer.EndArray();
            writer.EndObject();
        }

        void Deserialize(const Value& rootValue){
            if(rootValue.IsObject()){
                this->publisher = rootValue["publisher"].GetString();
                this->name = rootValue["name"].GetString();
                this->version = rootValue["version"].GetString();
                this->signature = rootValue["signature"].GetString();
                const Value& states = rootValue["states"];
                for (Value::ConstValueIterator itr = states.Begin(); itr != states.End(); ++itr){
                    this->states.push_back(itr->GetInt());
                }
                if (rootValue.HasMember("attrs"))
                {
                    const Value& attrs = rootValue["attrs"];
                    if (attrs.IsArray())
                    {
                        for (rapidjson::SizeType i = 0; i < attrs.Size(); i++)
                        {
                            Action attr;
                            attr.Deserialize(attrs[i], "set");
                            this->attrs.push_back(attr);
                        }
                    }
                }

                if (rootValue.HasMember("dependencies"))
                {
                    const Value& dependencies = rootValue["dependencies"];
                    if (dependencies.IsArray())
                    {
                        for (rapidjson::SizeType i = 0; i < dependencies.Size(); i++)
                        {
                            Action dep;
                            dep.Deserialize(dependencies[i], "depend");
                            this->dependencies.push_back(dep);
                        }
                    }
                }
            }
        }

    };

};
#endif //PKG6_PACKAGEINFO_H
