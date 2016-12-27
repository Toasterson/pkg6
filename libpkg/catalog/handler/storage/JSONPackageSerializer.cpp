//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#include "JSONPackageSerializer.h"

void JSONPackageSerializer::Serialize(const pkg::PackageInfo &pkg, Value &val) {
    if(!val.IsObject()) {
        val.SetObject();
    }
    val.AddMember<string>(StringRef("publisher"), pkg.publisher, alloc);
    val.AddMember<string>(StringRef("name"), pkg.name, alloc);
    val.AddMember<string>(StringRef("version"), pkg.version, alloc);
    val.AddMember<string>(StringRef("build"), pkg.build_release, alloc);
    val.AddMember<string>(StringRef("branch"), pkg.branch, alloc);
    val.AddMember<string>(StringRef("packaging_date"), pkg.getPackagingDate(), alloc);

    if(!pkg.signature.empty()) {
        val.AddMember<string>(StringRef("signature"), pkg.signature, alloc);
    }
    val.AddMember<string>(StringRef("summary"), pkg.summary, alloc);
    if(!pkg.description.empty()) {
        val.AddMember<string>(StringRef("description"), pkg.description, alloc);
    }
    if(!pkg.humanversion.empty()){
        val.AddMember<string>(StringRef("humanversion"), pkg.humanversion, alloc);
    }
    Value classification(kArrayType);
    for(auto classi : pkg.classification){
        classification.PushBack(Value(StringRef(classi), alloc), alloc);
    }
    val.AddMember(StringRef("classifications"), classification, alloc);
    Value states(kArrayType);
    for(auto state : pkg.states){
        states.PushBack<int>(state, alloc);
    }
    val.AddMember(StringRef("states"), states, alloc);
    if(!pkg.attrs.empty()){
        Value attrs(kArrayType);
        for(auto attr : pkg.attrs) {
            if (!attr.name.empty()) {
                Value valAttr(kObjectType);
                SerializeAttributeAction(attr, valAttr);
                attrs.PushBack(valAttr, alloc);
            }
        }
        val.AddMember(StringRef("attrs"), attrs, alloc);

    }
    if(!pkg.dependencies.empty()){
        Value deps(kArrayType);
        for(auto dep : pkg.dependencies){
            Value valDep(kObjectType);
            SerializeDependencyAction(dep, valDep);
            deps.PushBack(valDep, alloc);
        }
        val.AddMember(StringRef("dependencies"), deps, alloc);
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

void JSONPackageSerializer::SerializeAttributeAction(const AttributeAction &attr, Value &val) {
    val.SetObject();
    Value valValues(kArrayType);
    for (auto value : attr.values) {
        valValues.PushBack(StringRef(value), alloc);
    }
    val.AddMember(StringRef(attr.name), valValues, alloc);
    if (!attr.optionals.empty()) {
        Value valOpt(kObjectType);
        for (auto item: attr.optionals) {
            valOpt.AddMember<string>(StringRef(item.first), item.second, alloc);
        }
        val.AddMember(StringRef("opt"), valOpt, alloc);
    };
}

void JSONPackageSerializer::SerializeDependencyAction(const DependAction &dep, Value &val) {
    val.SetObject();
    val.AddMember<string>(StringRef("fmri"), dep.fmri, alloc);
    val.AddMember<string>(StringRef("type"), dep.type, alloc);
    if(!dep.predicate.empty()){
        val.AddMember<string>(StringRef("predicate"), dep.predicate, alloc);
    }
    if(!dep.optional.empty()){
        Value valOpt(kObjectType);
        for(std::pair<string,string> opt : dep.optional){
            valOpt.AddMember<string>(StringRef(opt.first), opt.second, alloc);
        }
        val.AddMember(StringRef("opt"), valOpt, alloc);
    }
}

pkg::action::AttributeAction JSONPackageSerializer::DeserializeAttributeAction(const Value &rootValue) {
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

pkg::action::DependAction JSONPackageSerializer::DeserializeDependAction(const Value &rootValue) {
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
