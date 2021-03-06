//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_ACTION_H
#define PKG6_ACTION_H

#include <string>
#include <vector>

namespace pkg{
    class Action {
    public :
        Action(){}
        explicit Action(const std::string& type): action_type(type){}
        std::string action_type;

        std::vector<std::string> tokenize(std::string action_string);

        virtual void parseActionString(std::string action_string)= 0;

        virtual std::string toActionString()= 0;

        virtual void install()= 0;

        virtual bool validate()= 0;
    };
}



#endif //PKG6_ACTION_H
