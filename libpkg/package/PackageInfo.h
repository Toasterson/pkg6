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
#include "PackageCategory.h"
#include "image/LicenseInfo.h"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <action/Action.h>
#include <action/AttributeAction.h>
#include <action/DependAction.h>
#include <action/DirectoryAction.h>
#include <action/FileAction.h>

using namespace rapidjson;
using namespace  pkg::action;

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
        std::string summary;
        std::string description;
        bool obsolete;
        bool renamed;
        std::vector<std::string> classification;
        std::string humanversion;

        int size;
        int csize;

        std::string name;
        std::vector<LicenseInfo> licenses;
        std::tm last_update;
        std::tm last_install;

        std::vector<AttributeAction> attrs;
        std::vector<DependAction> dependencies;
        std::vector<DirectoryAction> dirs;
        std::vector<AttributeAction> links;
        std::vector<FileAction> files;

        PackageInfo(){}
        PackageInfo(const std::string& publisher, const std::string& name, const std::string& version): publisher(publisher), name(name), version(version){}
        PackageInfo(const PackageInfo& origin): publisher(origin.publisher), name(origin.name), version(origin.version){}
        PackageInfo(const std::string& FMRI){
            setFmri(FMRI);
        }

        std::string getFmri() const {
            return publisher + "/" + name + "@" + version;
        }

        std::string getFilePath() {
            return getFmri() + ".json";
        }

        void setFmri(const std::string& fmri);

        void addAction(const std::string& action_string);

        void commitManifest(istream& manifest);

        void markObsolete();

        void markRenamed();

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
            if(!signature.empty()) {
                writer.String("signature");
                writer.String(signature.c_str());
            }
            writer.String("summary");
            writer.String(summary.c_str());
            if(!description.empty()) {
                writer.String("description");
                writer.String(description.c_str());
            }
            if(!humanversion.empty()){
                writer.String("humanversion");
                writer.String(humanversion.c_str());
            }
            writer.String("classifications");
            writer.StartArray();
            for(auto classi : classification){
                writer.String(classi);
            }
            writer.EndArray();
            writer.String("states");
            writer.StartArray();
            for(int state : states){
                writer.Int(state);
            }
            writer.EndArray();

            if(!attrs.empty()) {
                writer.String("attrs");
                writer.StartArray();
                for (auto attr : attrs) {
                    attr.Serialize(writer);
                }
                writer.EndArray();
            }

            if(!dependencies.empty()) {
                writer.String("dependencies");
                writer.StartArray();
                for (auto dep : dependencies) {
                    dep.Serialize(writer);
                }
                writer.EndArray();
            }

            writer.EndObject();
        }

        void Deserialize(const Value& rootValue){
            if(rootValue.IsObject()){
                this->publisher = rootValue["publisher"].GetString();
                this->name = rootValue["name"].GetString();
                this->version = rootValue["version"].GetString();
                if(rootValue.HasMember("signature")) {
                    this->signature = rootValue["signature"].GetString();
                }
                this->summary = rootValue["summary"].GetString();
                if(rootValue.HasMember("description")) {
                    this->description = rootValue["description"].GetString();
                }
                if(rootValue.HasMember("humanversion")) {
                    this->humanversion = rootValue["humanversion"].GetString();
                }
                const Value& states = rootValue["states"];
                for (auto itr = states.Begin(); itr != states.End(); ++itr){
                    this->states.push_back(itr->GetInt());
                }
                const Value& classes = rootValue["classifications"];
                for (auto itr = classes.Begin(); itr != classes.End(); ++itr){
                    this->classification.push_back(itr->GetString());
                }
                if (rootValue.HasMember("attrs"))
                {
                    const Value& attrs = rootValue["attrs"];
                    if (attrs.IsArray())
                    {
                        for (rapidjson::SizeType i = 0; i < attrs.Size(); i++)
                        {
                            AttributeAction attr;
                            attr.Deserialize(attrs[i]);
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
                            DependAction dep;
                            dep.Deserialize(dependencies[i]);
                            this->dependencies.push_back(dep);
                        }
                    }
                }
            }
        }

    };

};
#endif //PKG6_PACKAGEINFO_H
