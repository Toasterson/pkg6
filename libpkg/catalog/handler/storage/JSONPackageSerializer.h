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
    void Serialize(const pkg::PackageInfo &pkg, Writer &writer) const;
    template <typename Writer>
    void SerializeAttributeAction(const AttributeAction &attr, Writer &writer) const;
    template <typename Writer>
    void SerializeDependencyAction(const DependAction &dep, Writer &writer) const;
    pkg::PackageInfo Deserialize(Value &rootValue);
    pkg::action::AttributeAction DeserializeAttributeAction(const Value &rootValue);
    pkg::action::DependAction DeserializeDependAction(const Value &rootValue);
};


#endif //PKG6_JSONPACKAGESERIALIZER_H
