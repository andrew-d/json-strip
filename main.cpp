#include "strip.hpp"

#include <iostream>
#include <istream>
#include <ostream>
#include <iterator>


int main(int argc, char* argv[]) {
    bool whitespace;

    if (argc == 1) {
        whitespace = true;
    } else if (argc == 2) {
        std::string arg(argv[1]);

        if (arg == "--no-whitespace") {
            whitespace = false;
        } else if (arg == "--whitespace") {
            whitespace = true;
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Usage: " << argv[0] << " [--[no-]whitespace]" << std::endl;
        return 1;
    }

    // Don't skip whitespace while reading
    std::cin >> std::noskipws;

    // Use stream iterators to copy the stream to a string
    std::istream_iterator<char> it(std::cin);
    std::istream_iterator<char> end;
    std::string results(it, end);

    // Strip it
    auto stripped = stripComments(results, whitespace);
    std::cout << stripped;
    return 0;
}
