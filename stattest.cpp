#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <sys/stat.h>

int main(int argc, char** argv)
{
    std::string fpath = argv[1];

    struct stat st;

    if(stat(fpath.data(),&st) != 0)
    {
        perror("oops");
        exit(-1);
    }

    printf("UID: %x\n",st.st_uid);
    printf("GID: %x\n",st.st_gid);

    bool isFolder = st.st_mode & S_IFDIR;
    
    bool x_a = S_IXOTH;
    bool x_g = S_IXGRP && (geteuid() == st.st_gid);
    bool x_o = S_IXUSR && (geteuid() == st.st_uid);

    bool x = x_a || x_g || x_o;

    std::cout << "isFolder: " << isFolder << std::endl;
    std::cout << "x: " << x << std::endl;

    return 0;
}