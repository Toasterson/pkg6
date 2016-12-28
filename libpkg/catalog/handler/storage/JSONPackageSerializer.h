//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_JSONPACKAGESERIALIZER_H
#define PKG6_JSONPACKAGESERIALIZER_H


#include <package/PackageInfo.h>

class JSONPackageSerializer {
public:
    template <typename Writer>
    void Serialize(const pkg::PackageInfo &pkg, Writer &writer) const{
        writer.StartObject();
        writer.String("publisher");
        writer.String(pkg.publisher.c_str());
        writer.String("name");
        writer.String(pkg.name.c_str());
        writer.String("version");
        writer.String(pkg.version.c_str());
        writer.String("build");
        writer.String(pkg.build_release.c_str());
        writer.String("branch");
        writer.String(pkg.branch.c_str());
        writer.String("packaging_date");
        writer.String(pkg.getPackagingDate());
        if(!pkg.signature.empty()) {
            writer.String("signature");
            writer.String(pkg.signature.c_str());
        }
        writer.String("summary");
        writer.String(pkg.summary.c_str());
        if(!pkg.description.empty()) {
            writer.String("description");
            writer.String(pkg.description.c_str());
        }
        if(!pkg.humanversion.empty()){
            writer.String("humanversion");
            writer.String(pkg.humanversion.c_str());
        }
        writer.String("classifications");
        writer.StartArray();
        for(auto classi : pkg.classification){
            writer.String(classi);
        }
        writer.EndArray();
        writer.String("states");
        writer.StartArray();
        for(int state : pkg.states){
            writer.Int(state);
        }
        writer.EndArray();

        if(!pkg.attrs.empty()) {
            writer.String("attrs");
            writer.StartArray();
            for (auto attr : pkg.attrs) {
                SerializeAttributeAction(attr, writer);
            }
            writer.EndArray();
        }

        if(!pkg.dependencies.empty()) {
            writer.String("dependencies");
            writer.StartArray();
            for (auto dep : pkg.dependencies) {
                SerializeDependencyAction(dep, writer);
            }
            writer.EndArray();
        }

        writer.EndObject();
    }

    template <typename Writer>
    void SerializeAttributeAction(const AttributeAction &attr, Writer &writer) const{
        if(!attr.name.empty()) {
            writer.StartObject();
            writer.String(attr.name.c_str());
            writer.StartArray();
            for (auto value : attr.values) {
                writer.String(value.c_str());
            }
            if (!attr.optionals.empty()) {
                writer.String("opt");
                writer.StartObject();
                for (auto pair: attr.optionals) {
                    writer.String(pair.first.c_str());
                    writer.String(pair.second.c_str());
                }
                writer.EndObject();
            }
            writer.EndArray();
            writer.EndObject();
        }
    }

    template <typename Writer>
    void SerializeDependencyAction(const DependAction &dep, Writer &writer) const{
        writer.StartObject();
        writer.String("fmri");
        writer.String(dep.fmri.c_str());
        writer.String("type");
        writer.String(dep.type.c_str());
        if(!dep.predicate.empty()){
            writer.String("predicate");
            writer.String(dep.predicate.c_str());
        }
        if(!dep.optional.empty()){
            for(std::pair<std::string,std::string> opt : dep.optional){
                writer.String(opt.first.c_str());
                writer.String(opt.second.c_str());
            }
        }
        writer.EndObject();
    }

    pkg::PackageInfo Deserialize(Value &rootValue){
        pkg::PackageInfo pkg;
        if(rootValue.IsObject()){
            pkg.publisher = rootValue["publisher"].GetString();
            pkg.name = rootValue["name"].GetString();
            pkg.version = rootValue["version"].GetString();
            pkg.build_release = rootValue["build"].GetString();
            pkg.branch = rootValue["branch"].GetString();
            pkg.setPackagingDate(rootValue["packaging_date"].GetString());
            if(rootValue.HasMember("signature")) {
                pkg.signature = rootValue["signature"].GetString();
            }
            pkg.summary = rootValue["summary"].GetString();
            if(rootValue.HasMember("description")) {
                pkg.description = rootValue["description"].GetString();
            }
            if(rootValue.HasMember("humanversion")) {
                pkg.humanversion = rootValue["humanversion"].GetString();
            }
            const Value& states = rootValue["states"];
            for (auto itr = states.Begin(); itr != states.End(); ++itr){
                pkg.states.push_back(itr->GetInt());
            }
            const Value& classes = rootValue["classifications"];
            for (auto itr = classes.Begin(); itr != classes.End(); ++itr){
                pkg.classification.push_back(itr->GetString());
            }
            if (rootValue.HasMember("attrs"))
            {
                const Value& attrs = rootValue["attrs"];
                if (attrs.IsArray())
                {
                    for (rapidjson::SizeType i = 0; i < attrs.Size(); i++)
                    {
                        pkg.attrs.push_back(DeserializeAttributeAction(attrs[i]));
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
                        pkg.dependencies.push_back(DeserializeDependAction(dependencies[i]));
                    }
                }
            }
        }
        return pkg;
    }

    pkg::action::AttributeAction DeserializeAttributeAction(const Value &rootValue){
        AttributeAction attr;
        if(rootValue.IsObject()){
            for(auto itr = rootValue.MemberBegin(); itr != rootValue.MemberEnd(); ++itr){
                if(itr->name.GetString() == "opt"){
                    for(auto itr2 = itr->value.MemberBegin(); itr2 != itr->value.MemberEnd(); ++itr2){
                        attr.optionals.insert(std::pair<std::string,std::string>(itr2->name.GetString(), itr2->value.GetString()));
                    }
                } else {
                    attr.name = itr->name.GetString();
                    if (itr->value.IsArray()) {
                        for (rapidjson::SizeType i = 0; i < itr->value.Size(); i++) {
                            attr.values.push_back(itr->value[i].GetString());
                        }
                    }
                }
            }
        }
        return attr;
    }

    pkg::action::DependAction DeserializeDependAction(const Value &rootValue){
        DependAction dep;
        if(rootValue.IsObject()){
            for(Value::ConstMemberIterator itr = rootValue.MemberBegin(); itr != rootValue.MemberEnd(); ++itr){
                std::string name = itr->name.GetString();
                std::string value = itr->value.GetString();
                if(name == "fmri"){
                    dep.fmri = value;
                } else if(name == "type"){
                    dep.type = value;
                } else if(name == "predicate"){
                    dep.predicate = value;
                } else {
                    dep.optional.insert(std::pair<std::string,std::string>(name, value));
                }
            }
        }
        return dep;
    }
};


#endif //PKG6_JSONPACKAGESERIALIZER_H
