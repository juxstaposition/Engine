#include "Application.hpp"

int main(int argc, char* argv[]) {
    try {

        auto app = Application();

        app.start();

    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
