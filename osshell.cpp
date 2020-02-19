#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm> // for std::min
#include <fstream>	// for file reading

std::vector<std::string> splitString(std::string text, char d);
std::string getFullPath(std::string cmd, const std::vector<std::string>& os_path_list);
bool fileExists(std::string full_path, bool *executable);
void init_hist( std::vector<std::string> *hist );
std::string nums = "0123456789";

int main (int argc, char **argv)
{

	//current history vector
	std::vector<std::string> hist;

    std::string input;
	std::vector<std::string> input_list; // vector of input delimited by spaces
    char* os_path = getenv("PATH");
    std::vector<std::string> os_path_list = splitString(os_path, ':');

	//initialize hist vector
	init_hist( &hist);

    std::cout << "Welcome to OSShell! Please enter your commands ('exit' to quit)." << std::endl;

    while(1){

        printf("osshell> ");

        getline(std::cin,input);
		input_list = splitString(input, ' ');

// ------------------------------ EXIT ------------------------------- //
        
        if(input.compare("exit") == 0)
        {

			// clear history file, then write back
			std::ofstream out;
			out.open("./history.txt", std::ofstream::out | std::ofstream::trunc);
			out.close();

			out.open("./history.txt");

			for(int i=0; i< hist.size(); i++){

				out << hist[i] << "\n";
				
			}

            break;  //exits the shell
        }

// ------------------------------ HISTORY ------------------------------- //
        
        else if(input_list[0].compare("history") == 0)
        {

			//just called "history"
			if(input_list.size() == 1){

				for( int i=0; i < hist.size(); i++){

					std::cout << "  " << i << ": " << hist[i] << "\n";
				}

			}
			
			//called "history XXXX"
			else if( input_list.size() == 2 ){

				// called "history clear"
				if( input_list[1] == "clear" ){
				
					std::vector<std::string> empty_hist;
					hist = empty_hist;		

				}

				// called "history <int>"
				else{

					// check for "history <garbage>"
					for(int i=0; i< input_list[i].length(); i++){
						if( nums.find(input_list[1][i]) < 0){

							std::cout << "Error: history expects an integer > 0 (or 'clear')\n";
							continue;
						}
					}

					int length = std::stoi(input_list[1]);
					length = std::min(length, (int)hist.size());
		
					if( length < 1 && (int)hist.size() != 0){

						std::cout << "Error: history expects an integer > 0 (or 'clear')\n";
						continue;

					}

					else{
					
						for( int i=(int)hist.size() - length; i < (int)hist.size(); i++){

							std::cout << "  " << i << ": " << hist[i] << "\n";
						}
					}
				}
			}
		}


// ------------------------------ EXEC CALL ------------------------------- //


        else if(input.compare("") != 0)   //Structure adapted from https://stackoverflow.com/a/19461845
        {
            pid_t pid = fork(); //forks the process

            if(pid == 0)    //if process is child
            {
                // printf("child spawn\n");

                std::vector<std::string> tokens = splitString(input, ' ');  //splits input by spaces to approximate argv[]

                char** new_argv = (char**) malloc(tokens.size() * sizeof(char*));

                for(int i = 0;i < tokens.size();i++)
                {
                    new_argv[i] = (char*) tokens[i].data();
                }
                
                // for(int i = 0;i < tokens.size();i++)
                // {
                //     printf("Element %d: %s\n",i,new_argv[i]);
                // }

                execv(new_argv[0],new_argv);
                exit(0);
            }

            else if (pid > 0)   //process is parent
            {
                int status = 0;
                waitpid(0,&status,0);   //waits for child to finish, places exit code into status

                // printf("parent continues\n");

                if(status != 0)
                {
                    perror("Error occurred during child spawn");    //prints error message and exits
                    exit(-1);
                }
            }  
        }

		//write last command to history
		hist.push_back( input );

		if(hist.back().compare("history clear") == 0){
			hist.erase(hist.end());
		}

		if( hist.size() > 128 ){
			hist.erase(hist.begin());
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
    *executable = false;
    return false;
}


void init_hist( std::vector<std::string> *hist ){

	std::ifstream hist_file("./history.txt");

	// if hist file doesnt exist
	if( hist_file.fail() ){

		hist_file.close();
		std::ofstream new_hist("./history.txt");
		new_hist.close();

	}

	if( !hist_file.is_open() ) hist_file.open("./history.txt");
	std::string temp;

	while(std::getline( hist_file, temp)){
		
		hist->push_back(temp);
	}
	hist_file.close();

}
