//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "JSONPackageSerializer.h"

void JSONPackageSerializer::Serialize(const pkg::PackageInfo &pkg, Value &val) {
    val["publisher"].SetString(StringRef(pkg.publisher));
    val["name"].SetString(StringRef(pkg.name));
    val["version"].SetString(StringRef(pkg.version));
    val["build"].SetString(StringRef(pkg.build_release));
    val["branch"].SetString(StringRef(pkg.branch));
    val["packaging_date"].SetString(StringRef(pkg.getPackagingDate()));
    if(!pkg.signature.empty()) {
        val["signature"].SetString(StringRef(pkg.signature));
    }
    val["summary"].SetString(StringRef(pkg.summary));
    if(!pkg.description.empty()) {
        val["description"].SetString(StringRef(pkg.description));
    }
    if(!pkg.humanversion.empty()){
        val["humanversion"].SetString(StringRef(pkg.humanversion));
    }
    Value classification(kArrayType);
    for(int i = 0; i < pkg.classification.size(); i++){
        classification[i].SetString(StringRef(pkg.classification[i]));
    }
    val["classification"] = classification;
    Value states(kArrayType);
    for(int i = 0; i < pkg.states.size(); i++){
        states[i].SetInt(pkg.states[i]);
    }
    val["states"] = states;
    if(!pkg.attrs.empty()){
        Value attrs(kObjectType);
        SerializeAttributeAction(pkg, attrs);
        val["attrs"] = attrs;
    }
    if(!pkg.dependencies.empty()){
        Value deps(kObjectType);
        SerializeDependencyAction(pkg, deps);
        val["dependencies"] = deps;
    }
}

pkg::PackageInfo JSONPackageSerializer::Deserialize(Value &rootValue) {
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
                    AttributeAction attr;
                    attr.Deserialize(attrs[i]);
                    pkg.attrs.push_back(attr);
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
                    pkg.dependencies.push_back(dep);
                }
            }
        }
    }
    return pkg;
}

void JSONPackageSerializer::SerializeAttributeAction(const pkg::PackageInfo &pkg, Value &val) {

}

void JSONPackageSerializer::SerializeDependencyAction(const pkg::PackageInfo &pkg, Value &val) {

}
