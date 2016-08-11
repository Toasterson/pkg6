#include <iostream>
#include <image/Image.h>

using namespace std;
using namespace pkg;

int main() {
    //Image img = Image("/home/toast/workspace/illumos/ips/pkg5/");
    //img.upgrade_format("/home/toast/workspace/illumos/ips/pkg6");

    AttributeAction action1(std::string("set name=variant.opensolaris.zone value=global value=nonglobal"));
    AttributeAction action2(std::string("set name=variant.arch value=i386"));
    AttributeAction action3(std::string("set name=pkg.summary value=\"Gujarati language support\""));
    AttributeAction action4(std::string("set name=pkg.summary value=\\\"'XZ Utils - loss-less file compression application and library.'\\\""));
    AttributeAction action5(std::string("set name=pkg.summary value=\\\"provided mouse accessibility enhancements\\\""));
    AttributeAction action6(std::string("set name=info.upstream value=X.Org Foundation"));
    AttributeAction action7(std::string("set name=pkg.description value=Latvian language support's extra files"));
    AttributeAction action8(std::string("set name=illumos-gate.info.git-remote value=git://github.com/illumos/illumos-gate.git"));



    cout << endl << "All Done!" << endl;
    return 0;
}