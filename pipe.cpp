#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <sys/wait.h>

#define MAX_PIPE 1000
#define MAX_ARGC 1000
#define BUFF_SIZE 100

void exec(std::vector<std::string> cmd, int fd_in, int fd_out)
{
    char *argv[MAX_ARGC];
    for (int i = 0; i < cmd.size(); i++)
    {
        //std::cout << "argv[" << i << "]: " << cmd.at(i) << std::endl;
        argv[i] = (char *)cmd.at(i).c_str();
    }
    argv[cmd.size()] = NULL;

    int status = 0;
    pid_t pid = fork();
    if (pid == 0)
    {
        dup2(fd_in,  STDIN_FILENO);
        dup2(fd_out, STDOUT_FILENO);
        execvp(argv[0], argv);
        return;
    }
    else
    {
        waitpid((pid_t)pid, &status, 0);
        return;
    }
}

void exec_pipe(
    std::vector<std::vector<std::string>> cmd,
    std::string file_in,
    std::vector<std::ofstream> &fouts
)
{
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);
    char buffer_in[BUFF_SIZE];
    int len_in = 0;
    for(int i = 0; i < cmd.size(); i++)
    {
        close(p1[1]);
        exec(cmd.at(i), p1[0], p2[1]);
        close(p1[0]);

        p1[0] = p2[0];
        p1[1] = p2[1];
        pipe(p2);
    }
    close(p1[1]);
    close(p2[0]);
    close(p2[1]);

    char buffer[BUFF_SIZE];
    int len = 0;
    while(len = read(p1[0], buffer, BUFF_SIZE-1))
    {
        //printf("%d", len);
        buffer[len] = '\0';
        std::cout << buffer;
    }

    close(p1[0]);
//dup2(stdin_copy, STDIN_FILENO);
}

int main()
{
    while (true)
    {
        std::string str_in = "";
        std::string str_temp = "";
        std::vector<std::vector<std::string>> cmd;
        std::vector<std::string> cmd_temp;
        std::vector<std::string> file_ins;
        std::vector<std::ofstream> fouts;
        std::stringstream ss;

        std::cout << "Please input your command: ";

        std::getline(std::cin, str_in, '\n');
        ss.str(str_in);
        while(ss >> str_temp)
        {
            cmd_temp.push_back(str_temp);
        }

        cmd.push_back(std::vector<std::string>());
        for(int i = 0; i < cmd_temp.size(); i++)
        {
            if (cmd_temp.at(i) == "|")
            {
                cmd.push_back(std::vector<std::string>());
            }
            else
                cmd.back().push_back(cmd_temp.at(i));
        }
        exec_pipe(cmd, "", fouts);

        //break;
    }
}


