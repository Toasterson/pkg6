#include <iostream>
#include <Image.h>

using namespace std;
using namespace pkg;

int main() {
    Image img = Image("/home/toast/workspace/cpp/pkg6/sample_data/pkg5");
    img.upgrade_format("/home/toast/workspace/cpp/pkg6/sample_data/pkg6");

    cout << "All Done!" << endl;
    return 0;
}