#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>

std::vector<std::string> splitString(std::string text, char d);
std::string getFullPath(std::string cmd, const std::vector<std::string>& os_path_list);
bool fileExists(std::string full_path, bool *executable);
char** vectorToArray(std::vector<std::string> input);

int main (int argc, char **argv)
{
    std::string input;
    char* os_path = getenv("PATH");
    std::vector<std::string> os_path_list = splitString(os_path, ':');

    std::cout << "Welcome to OSShell! Please enter your commands ('exit' to quit)." << std::endl;

    while(1)
    {
        printf("osshell> ");

        getline(std::cin,input);
        
        if(input.compare("exit") == 0)  //exits the shell
        {
            break;
        }
        
        else if(input.compare("history") == 0)
        {
            //TODO execute history command
        }

        else if(input.compare("") != 0)   //Structure adapted from https://stackoverflow.com/a/19461845
        {
            pid_t pid = fork(); //forks the process

            if(pid == 0)    //if current process is child
            {
                std::vector<std::string> tokens = splitString(input, ' ');  //splits input by spaces to approximate argv[]

                char** new_argv = vectorToArray(tokens);    //converts vector to a char**

                std::string command = tokens[0];

                if(command[0] == '.')   //relative path
                {
                    //relative path handling
                }

                else if(command[0] == '/')  //full path
                {
                    //full path handling
                }

                else    //expects a file in PATH
                {
                    //test if it's found in the PATH

                    // if(fileNotFound)
                    //     printf("<%s>: Error running command\n",input.substr(0,input.find(' ')).data());
                }

                execv(command.data(),new_argv);
                exit(0);
            }

            else if (pid > 0)   //process is parent
            {
                int status = 0;
                waitpid(0,&status,0);   //waits for child to finish, places exit code into status

                // if(status != 0)
                // {
                //     printf("<%s>: Error running command\n",input.substr(0,input.find(' ')).data());    //prints error message and exits
                //     exit(-1);   //TODO do we want to exit here
                // }
            }  
        }//else if(input.compare("") != 0)
    }//while(1)


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

    int start = 0;
    int size = 0;

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

// Returns a string for the full path of a command if it is found in PATH, otherwise simply return ""
std::string getFullPath(std::string cmd, const std::vector<std::string>& os_path_list)
{
    return "";
}

// Returns whether a file exists or not; should also set *executable to true/false 
// depending on if the user has permission to execute the file
bool fileExists(std::string full_path, bool *executable)
{
    std::ifstream file(full_path.data());

    bool result = file.good();

    if(result)
        *executable = ! access(full_path.data(),X_OK);

    return result;
}

// converts std::string vector to a char**
char** vectorToArray(std::vector<std::string> input)
{
    char** result = (char**) malloc(input.size() * sizeof(char*));

    for(int i = 0;i < input.size();i++)
    {
        result[i] = (char*) input[i].data();
    }

    return result;
}