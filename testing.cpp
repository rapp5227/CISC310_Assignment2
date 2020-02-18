#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

std::vector<std::string> splitString(std::string text, char d)
{
    std::vector<std::string> result;

    int start = 0;
    int size = 0;

    text = "test_test_shenanigans_test__";
    d = '_';

    for(int i = 0;i < text.length();i++)
    {
        ++size;

        if(text[i] == d)
        {
            result.push_back(text.substr(start,size-1));

            start = i + 1;
            size = 0;
        }
    }
    
    result.push_back(text.substr(start,size));

    return result;
}

int main()
{
    char* path = "/bin/ls";
    char** argv = (char**) malloc(sizeof(char**));

    argv[0] = path;

    pid_t pid = fork();

    if(pid == 0)
    {
        execv(path,argv);
    }

    else
    {
        int status = 0;
        waitpid(0,&status,0);
        sleep(2);
        printf("child is dead\n");
    }

    return 0;
}
