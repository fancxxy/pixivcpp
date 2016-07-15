#include "CmdlineOption.h"

bool isOption(const std::string &s) {
    return ((s.size() >= 3 && s[0] == '-' && s[1] == '-' && s[2] != '-') || 
            (s.size() == 2 && s[0] == '-' && s[1] != '-'));
}

CmdlineOption::CmdlineOption(int argc, char *argv[]) {
    std::vector<std::string> raw(argv + 1, argv + argc);
    std::string option;
    for (auto &v : raw) {
        if (isOption(v)) {
            arguments_[v];
            option = v;
        } else {
            if (!option.empty())
                arguments_[option].push_back(v);
        }

    }
}

bool CmdlineOption::hasOption(const std::string &option) const {
    return (arguments_.end() != arguments_.find(option));
}

const std::vector<std::string>& CmdlineOption::getArguments(const std::string &option) {
    static const std::vector<std::string> empty;
    return hasOption(option) ? arguments_[option] : empty;
}

