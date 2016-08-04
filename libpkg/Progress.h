//
// Copyright (c) Till Wegmueller 2016 under CDDL
// for License see LICENSE file in root of repository
//

#ifndef PKG6_PROGRESS_H
#define PKG6_PROGRESS_H

#include <iostream>

class Progress {

private:
    std::ostream& stream;
    int max;
    int current;
    static const int statusprint = 5;
    std::string pre_text;
    std::string post_text;
    void writeStatus(){
        stream << '\r' << pre_text << '\t' << current << '/' << max << '\t' << post_text << std::flush;
    }
public:
    Progress(const std::string& pre_text = "", const std::string& post_text = "", const int& maximum = 100):
            stream(std::cout),
            pre_text(pre_text),
            post_text(post_text),
            max(maximum),
            current(0)
    { }
    Progress(std::ostream& outstream, const std::string& pre_text = "", const std::string& post_text = "", const int& maximum = 100):
            stream(outstream),
            pre_text(pre_text),
            post_text(post_text),
            max(maximum),
            current(0)
    { }
    void setMax(const int& max){this->max=max;}
    int operator+=(const int& count){
        current += count;
        if(current % statusprint == 0) {
            writeStatus();
        }
        if(current == count){
            writeStatus();
            stream << "\n";
        }
        return current;
    }

    int operator++(int){ return operator+=(1);}


};


#endif //PKG6_PROGRESS_H
