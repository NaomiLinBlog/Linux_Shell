#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <signal.h>
#include <vector>
#include <algorithm>
#include <sys/wait.h>

#define MAX_WORDS_IN_LINE 1000
#define BUFF_SIZE 100000

int main()
{
    int p1[2];
    int p2[2];
    int num = 0;
    int this_vbar = 0;
    int last_vbar = 0;
    int stdin_copy = dup(STDIN_FILENO);
    int stdout_copy = dup(STDOUT_FILENO);
    char pipe_buff[BUFF_SIZE] = {0};
    std::string str_in = "";
    std::string str_temp = "";
    std::vector<std::string> cmd;
    std::vector<int> vbar_pos;
    std::stringstream ss;
    int status;
    pid_t pid;

    while (true) {
	cmd.clear();
	ss.clear();

        std::cout << "Please input your command: ";

        /*get input from std::cin, and store it*/
        std::getline(std::cin, str_in, '\n');
        ss.str(str_in);
        while (ss >> str_temp)
        {
            cmd.push_back(str_temp); /*store splited strings in vector cmd*/
        }

        char *cmd2[1000];
        for (int i = 0; i < cmd.size(); i++) {
            //std::cout << "argv[" << i << "]: " << cmd.at(i) << std::endl;
            cmd2[i] = (char *)cmd.at(i).c_str();
        }

	pid = fork();
	if (pid == 0) {
            execvp(cmd[0].c_str(), cmd2);
	} else {
	    waitpid((pid_t)pid, &status, 0);
	}
    }
}


