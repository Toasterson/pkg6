#include <iostream>
#include <image/Image.h>

using namespace std;
using namespace pkg;

int main() {
    Image img = Image("/home/toast/workspace/illumos/ips/pkg5/");
    img.upgrade_format("/home/toast/workspace/illumos/ips/pkg6");

    cout << endl << "All Done!" << endl;
    return 0;
}