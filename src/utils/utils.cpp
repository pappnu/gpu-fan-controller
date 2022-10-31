#include "utils.hpp"
#include <algorithm>
#include <string>
#include <vector>

ArgumentParser::ArgumentParser(int &argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
        arguments.push_back(std::string(argv[i]));
    }
};

const std::string &
ArgumentParser::getArgument(const std::string &argumentName) {
    std::vector<std::string>::const_iterator itr =
        std::find(arguments.begin(), arguments.end(), argumentName);
    if (itr != arguments.end() && ++itr != arguments.end()) {
        return *itr;
    }
    return emptyString;
}

bool ArgumentParser::argumentExists(const std::string &argumentName) {
    return std::find(arguments.begin(), arguments.end(), argumentName) !=
           arguments.end();
}
