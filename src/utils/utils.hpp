#include <string>
#include <vector>

class ArgumentParser {
  private:
    std::vector<std::string> arguments;
    const std::string emptyString = std::string();

  public:
    ArgumentParser(int &argc, char **argv);
    const std::string &getArgument(const std::string &argumentName);
    bool argumentExists(const std::string &argumentName);
};
