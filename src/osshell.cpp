#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm> // for std::min
#include <fstream>	// for file reading
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
 
std::vector<std::string> splitString(std::string text, char d);
std::string getFullPath(std::string cmd, const std::vector<std::string>& os_path_list);
bool fileExists(std::string full_path, bool *executable);
std::string nums = "0123456789";

int main (int argc, char **argv)
{
	//read history file into hist array.
	std::ifstream hist_file("./history.txt");
	//copy of hist while program is running
	std::vector<std::string> hist;

    std::string input;
	std::vector<std::string> input_list;
    char* os_path = getenv("PATH");
    std::vector<std::string> os_path_list = splitString(os_path, ':');

// initialize hist vector
	if( hist_file.fail() ){

		hist_file.close();
		std::ofstream new_hist("./history.txt");
		new_hist.close();
	}

	if( !hist_file.is_open() ) hist_file.open("./history.txt");
	std::string temp;

	while(std::getline( hist_file, temp)){
		//std::cout << temp;
		hist.push_back(temp);
	}
	hist_file.close();


    std::cout << "Welcome to OSShell! Please enter your commands ('exit' to quit)." << std::endl;

    while( 1 ){

		std::cout << "osshell> ";
		std::getline(std::cin, input);

        //scrub quotation marks

        input_list = splitString(input,'"');
        input = "";

        for(int i = 0;i < input_list.size();i++)
        {
            input += input_list[i];
        }
        
		input_list = splitString(input, ' ');

		if( input.compare("") == 0){
			continue;
		}
		

// exit command
		if( input_list.front().compare("exit") == 0 ){
					
			hist.push_back( input );
			//write back to history file

			//clear file contents -- https://stackoverflow.com/questions/17032970/clear-data-inside-text-file-in-c
			std::ofstream out;
			out.open("./history.txt", std::ofstream::out | std::ofstream::trunc);
			out.close();

			out.open("./history.txt");

			for(int i=0; i< hist.size(); i++){
				
				out << hist[i] << "\n";
				
			}

			return 0;
		}
// history command
		else if( input_list.front().compare("history") == 0){

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

				// called "history <int>" or "history <garbage>"
				else{

					int length;

                    if(sscanf(input_list[1].data(),"%d",&length) != 1)
                    {
                        std::cout << "Error: history expects an integer > 0 (or 'clear')\n";
                        continue;
                    }

                    std::string lengthTest = std::to_string(length);

                    if(lengthTest.size() != input_list[1].size())
                    {
                        // std::cout << "test: " << lengthTest.size() << ", actual: " << input_list[1].size() << std::endl;

                        std::cout << "Error: history expects an integer > 0 (or 'clear')\n";
                        continue;
                    }

					length = std::min(length, (int)hist.size());
		
					if( length < 1 && (int)hist.size() != 0)
                    {

						std::cout << "Error: history expects an integer > 0 (or 'clear')\n";

					}

					else{
					
						for( int i=(int)hist.size() - length; i < hist.size(); i++){

							std::cout << "  " << i << ": " << hist[i] << "\n";
						}
					}
				}
			
			}

		}










































		//Check if Real Command input in /bin or /usr/bin etc. 
		//else if( ){};

		else{




			std::string path = getFullPath( input_list.front(), os_path_list );

            if(path.compare("") == 0)
                continue;

			char *argv[input_list.size()];
			int status;

			for(int i=0; i < input_list.size(); i++){

				argv[i] = (char*)input_list[i].c_str();
			}

			argv[input_list.size()] = NULL;

	/*
			std::cout << os_path_list.size() << std::endl;
			for(int i=0; i < os_path_list.size(); i++)  std::cout << "PATH list: " << os_path_list[i] << std::endl;
			std::cout << "path was: " << path << std::endl;
		 	std::cout << "argv: {";
			for( int i=0; i<input_list.size(); i++)		std::cout << argv[i] << ", ";
			std::cout << "}" << std::endl;

*/

			pid_t pid = fork();
			//std::cout << "pid was: " << pid << std::endl;

			if(pid == 0)
			{
				execv(path.c_str(),argv);
			}

			else
			{
				wait(&status);
			}

		/*	if(WIFEXITED(status) != 0){

				std::cout << "Exited with status: " << status << std::endl;
			}	
		*/		

		}











































		hist.push_back( input );

		if(hist.back().compare("history clear") == 0){
			hist.erase(hist.end());
		}

		if( hist.size() > 128 ){
			hist.erase(hist.begin());
		}

	}

		

		//  If command is `history` print previous N commands
		//  For all other commands, check if an executable by that name is in one of the PATH directories
		//   If yes, execute it
		//   If no, print error statement: "<command_name>: Error running command" (do include newline)

    return 0;
}

// Returns vector of strings created by splitting `text` on every occurance of `d`
std::vector<std::string> splitString(std::string text, char d)
{
	//result vector
    std::vector<std::string> result;
	// char index for next ' '
	int space = 0; 

    while(text.length() != 0){
        space = text.find(d);

        if(space < 0){

			result.push_back(text);
            text = "";
        }

		else{

			result.push_back(text.substr(0,space));
            text = text.substr(space + 1);   
        }
    }
    return result;
}

/*

// Returns a string for the full path of a command if it is found in PATH, otherwise simply return ""
std::string getFullPath(std::string cmd, const std::vector<std::string>& os_path_list)
{
	std::string result = "";
	bool can_run = true;

	for(int i=0; i< os_path_list.size(); i++){

		//std::cout << os_path_list[i] + "/" + cmd << std::endl;

		if( fileExists( os_path_list[i] + "/" + cmd, &can_run )){

			if(can_run){
				result = os_path_list[i] + "/" + cmd;
			}
		}
	}

    return result;
}

*/

std::string getFullPath(std::string cmd, const std::vector<std::string>& os_path_list)
{
    std::string result = "";
    bool x = false;
    bool exists = false;

    for(int i = 0;i < os_path_list.size();i++)
    {
        exists = fileExists((os_path_list[i] + "/" + cmd),&x);

        if(exists && x)
        {
            result = os_path_list[i] + "/" + cmd;
            break;
        }
    }

    if(result.compare("") == 0) //file not in path
    {
        exists = fileExists(cmd,&x);    //testing relative/full path

        if(exists && x) //full or relative path hit
        {
           // std::cout << "command exists on relative or full path: " << cmd << std::endl;

            result = cmd;
        }

        else    //no file located
            printf("%s: Error running command\n",cmd.data());
    }

    return result;
}
/*
// Returns whether a file exists or not; should also set *executable to true/false 
// depending on if the user has permission to execute the file
bool fileExists(std::string full_path, bool *executable)
{
	*executable = false;

	// if File exists
	if( access( full_path.c_str(), F_OK) == 0){

		// if user has execute perms
		if( access( full_path.c_str(), X_OK) == 0){
			*executable = true;
		}
		
		return true;

	}

	else return false;
}
*/
bool fileExists(std::string fpath, bool *executable)
{
    struct stat st;

    if(stat(fpath.data(),&st) != 0)
    {
        *executable = false;
        return false;
    }

    bool isFolder = st.st_mode & S_IFDIR;
    
    bool x_a = S_IXOTH;                                 //can anyone execute?
    bool x_g = S_IXGRP && (geteuid() == st.st_gid);     //is the user in a group that can execute?
    bool x_o = S_IXUSR && (geteuid() == st.st_uid);     //does the user own this file, and can they execute it?

    bool x = x_a || x_g || x_o;

    *executable = x && !isFolder;

    return true;
}//fileExists








