#include "pixiv/PixivCpp.h"

int main()
{
    //Cmdline cmd(argc, argv);
    
    PixivCpp pixiv("username", "password");

    pixiv.DownloadWorks("56640285");

    pixiv.DownloadWorks("56629877");

    return 0;
}

