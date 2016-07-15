#include "pixiv/PixivCpp.h"
#include "pixiv/CmdlineOption.h"
#include <iostream>

void showHelpInfo() {
    std::cout << " Usage: pixivcpp --pic ..."                                    << std::endl;
    std::cout << " eg:    pixivcpp --pic 57892762 57862790 56464401"             << std::endl;

    std::cout << "pixiv download program"                                        << std::endl;
    std::cout << " -h, --help"      << "\t\t" << "show help information"         << std::endl;
    std::cout << " -p, --picture"   << "\t\t" << "download one or more pictures" << std::endl;
    std::cout << " -f, --favorite"  << "\t\t" << "download favorite list"        << std::endl; 
}

int main(int argc, char *argv[])
{
    bool has_option = false;
    CmdlineOption cmd(argc, argv);

    if (cmd.hasOption("-h") || cmd.hasOption("--help")) {
        showHelpInfo();
        exit(EXIT_SUCCESS);
    }
    
    PixivCpp pixiv;

    if (cmd.hasOption("-f") || cmd.hasOption("--favorite")) {
        pixiv.DownloadFavorite();
        // add to favorite
        // delete from favorite
    }

    if ((has_option = cmd.hasOption("-p")) || cmd.hasOption("--picture")) {
        std::string option = has_option ? "-p" : "--picture";
        std::vector<std::string> pic_list = cmd.getArguments(option);
        for (const auto &s : pic_list) {
            pixiv.DownloadWorks(s);
        }
    }

    return 0;
}

