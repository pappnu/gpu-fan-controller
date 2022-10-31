#include "fancontroller.hpp"
#include "utils.hpp"
#include <csignal>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

void signalHandler(sigset_t signals, FanController &ctrller) {
    int signum;
    sigwait(&signals, &signum);
    std::cout << std::endl
              << "Caught signal " << signum << ". Exiting..." << std::endl;
    ctrller.stop();
}

template <class T>
void splitString(std::vector<T> &vec, const std::string &str,
                 const char &separator, T conversion(const std::string &)) {
    std::stringstream strStream(str);
    std::string segment;
    while (!std::getline(strStream, segment, separator).eof()) {
        vec.push_back(conversion(segment));
    }
}

int stringToInt(const std::string &str) { return std::stoi(str); }

void parseArguments(int &argc, char **argv, std::vector<int> &temperatures,
                    std::vector<int> &fanSpeeds, int &interval, int &hysteresis,
                    bool &verbose) {
    ArgumentParser parser = ArgumentParser(argc, argv);
    std::string temperaturesStr = parser.getArgument("--temperatures");
    splitString<int>(temperatures, temperaturesStr, ',', stringToInt);

    std::string fanSpeedsStr = parser.getArgument("--fan_speeds");
    splitString<int>(fanSpeeds, fanSpeedsStr, ',', stringToInt);

    interval = stringToInt(parser.getArgument("--interval"));
    hysteresis = stringToInt(parser.getArgument("--hysteresis"));
    verbose = parser.argumentExists("--verbose");
}

int main(int argc, char *argv[]) {
    sigset_t signals;
    sigemptyset(&signals);
    sigaddset(&signals, SIGINT);
    sigaddset(&signals, SIGTERM);
    // block signals in this and subsequently spawned threads
    pthread_sigmask(SIG_BLOCK, &signals, nullptr);

    std::vector<int> temperatures;
    std::vector<int> fanSpeeds;
    int interval;
    int hysteresis;
    bool verbose;

    try {
        parseArguments(argc, argv, temperatures, fanSpeeds, interval,
                       hysteresis, verbose);
    } catch (const std::exception &e) {
        std::cerr << "Failed to parse arguments: " << e.what() << std::endl;
    }

    if (!verbose) {
        // disable logging
        std::cout.rdbuf(nullptr);
    }

    FanController ctrller =
        FanController(temperatures, fanSpeeds, interval, hysteresis);
    std::thread exitSignalsThread =
        std::thread(signalHandler, std::ref(signals), std::ref(ctrller));

    try {
        ctrller.start();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unexpected error encountered. Terminating..."
                  << std::endl;
        ctrller.stop();
    }

    if (exitSignalsThread.joinable()) {
        exitSignalsThread.join();
    }
    return EXIT_SUCCESS;
}
