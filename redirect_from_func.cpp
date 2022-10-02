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
void redir_from();
int main()
{
    redir_from();
    return 0;
}
void redir_from()
{

    int pin[2];
    int pout[2];
    int num = 0;
    int stdin_copy = dup(STDIN_FILENO);
    int stdout_copy = dup(STDOUT_FILENO);
    char pipe_buff[BUFF_SIZE] = {0};
    //std::string cmd[1000][1000];
    std::string str_in = "";
    std::string str_temp = "";
    std::vector<std::string> cmd;
    std::vector<std::string> files;
    std::vector<int> vbar_pos;
    std::stringstream ss;
    int status;
    int ffd_in;
    pid_t pid;
    int len;
    while (true)
    {
        cmd.clear();
        ss.clear();
        files.clear();
        std::cout << "Please input your command: ";
        /*get input from std::cin, and store it*/
        std::getline(std::cin, str_in, '\n');
        ss.str(str_in);
        //pipe(pin);
        while (ss >> str_temp)
        {
            cmd.push_back(str_temp); /*store splited strings in vector cmd*/
        }
        for (int i = 0; i < cmd.size(); i++)
        {
            if(cmd.at(i)=="<")
            {
                files.push_back(cmd.at(i+1));
                //iter = i;
            }
        }
        char *cmd2[1000];
        for (int i = 0; i < cmd.size(); i++)
        {
            if(cmd.at(i)=="<")
                break;
            cmd2[i] = (char *)cmd.at(i).c_str();
        }
        for(int i=0; i<files.size(); i++)
        {
            pipe(pin);
            std::cout << "This is the " << i+1 << " th file."<< std::endl;
            ffd_in = fileno(fopen(files.at(i).c_str(),"rb+"));
            while(len=read(ffd_in,pipe_buff, sizeof(pipe_buff)-1))
                write(pin[1],pipe_buff,len);
            close(pin[1]);
            pid = fork();
            if (pid == 0)
            {
                dup2(pin[0],STDIN_FILENO);
                execvp(cmd[0].c_str(),cmd2);

            }
            else
            {
                close(pin[0]);
                waitpid((pid_t)pid, &status, 0);
            }

        }
    }

}

