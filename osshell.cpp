#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
 
std::vector<std::string> splitString(std::string text, char d);
std::string getFullPath(std::string cmd, const std::vector<std::string>& os_path_list);
bool fileExists(std::string full_path, bool *executable);
//branch testing
int main (int argc, char **argv)
{
    std::string input;
    char* os_path = getenv("PATH");
    std::vector<std::string> os_path_list = splitString(os_path, ':');

    std::cout << "Welcome to OSShell! Please enter your commands ('exit' to quit)." << std::endl;

    int shell_flag = 1;

    while(1)
    {
        printf("osshell> ");

        std::cin >> input;

        if(input.compare("exit") == 0)
        {
            break;  //exits the shell
        }
        
        else if(input.compare("history") == 0)
        {
            //TODO execute history command
        }

        else    //TODO this code was taken from the internet, I need to find the source in my browser history and provide it here
        {
            pid_t pid = fork(); //forks the process

            if(pid == 0)    //if process is child
            {
                //TODO implement execv() call here
                system(input.data());
                exit(0);
            }

            else if (pid > 0)   //process is parent
            {
                int status = 0;
                waitpid(0,&status,0);   //waits for child to finish

                if(status)
                {
                    //child process returned non-zero, some error
                    //TODO implement error handling?
                }
            }  
        }
        
    }
    // Repeat:
    //~  Print prompt for user input: "osshell> " (no newline)
    //~  Get user input for next command
    //~  If command is `exit` exit loop / quit program
    //  If command is `history` print previous N commands
    //  For all other commands, check if an executable by that name is in one of the PATH directories
    //   If yes, execute it
    //   If no, print error statement: "<command_name>: Error running command" (do include newline)

    return 0;
}

// Returns vector of strings created by splitting `text` on every occurance of `d`
std::vector<std::string> splitString(std::string text, char d)
{
    std::vector<std::string> result;

    return result;
}

// Returns a string for the full path of a command if it is found in PATH, otherwise simply return ""
std::string getFullPath(std::string cmd, const std::vector<std::string>& os_path_list)
{
    return "";
}

// Returns whether a file exists or not; should also set *executable to true/false 
// depending on if the user has permission to execute the file
bool fileExists(std::string full_path, bool *executable)
{
    *executable = false;
    return false;
}
