#pragma once

#include <map>
#include <string>
#include <vector>

class CmdlineOption
{
    public:
        CmdlineOption(int argc, char *argv[]);
        bool hasOption(const std::string &option) const;
        const std::vector<std::string>& getArguments(const std::string &option);

    private:
        std::map<std::string, std::vector<std::string>> arguments_;
};
