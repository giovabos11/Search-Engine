#include "SearchEngine.h"

using namespace std;

int main(int argc, char** argv) {
    // Create a new search engine object
    SearchEngine engine;

    // Set anticipated directory path to argument
    engine.setPath(argv[1]);

    // Execute user interface until they exit
    while(engine.menu()) {}

    return 0;
}
