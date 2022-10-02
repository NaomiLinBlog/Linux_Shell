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
using namespace std;
void redir_to();
int main()
{
    redir_to();
    return 0;
}
void redir_to()
{

    int p1[2];
    int p2[2];
    int num = 0;
    int this_vbar = 0;
    int last_vbar = 0;
    int stdin_copy = dup(STDIN_FILENO);
    int stdout_copy = dup(STDOUT_FILENO);
    char pipe_buff[BUFF_SIZE] = {0};
    string str_in = "";
    string str_temp = "";
    vector<std::string> cmd;
    vector<std::string> files;
    vector<int> vbar_pos;
    stringstream ss;
    int status;
    pid_t pid;
    int len;
    int ffd_in ;
    int ffd_out;
    FILE *pFile;
    while (true)
    {
        cmd.clear();
        ss.clear();
        files.clear();
        cout << "Please input your command: ";
        /*get input from std::cin, and store it*/
        getline(cin, str_in, '\n');
        ss.str(str_in);
        pipe(p1);
        while (ss >> str_temp)
        {
            cmd.push_back(str_temp); /*store splited strings in vector cmd*/
        }
        for (int i = 0; i < cmd.size(); i++)
        {
            if(cmd.at(i)==">")
                files.push_back(cmd.at(i+1));
        }
        char *cmd2[1000];
        for (int i = 0; i < cmd.size(); i++)
        {
            if(cmd.at(i)==">")
                break;
            cmd2[i] = (char *)cmd.at(i).c_str();
        }

        pid = fork();
        if (pid == 0)
        {
            dup2(p1[1],STDOUT_FILENO);
            execvp(cmd[0].c_str(), cmd2);
        }
        else
        {
            waitpid((pid_t)pid, &status, 0);
            close(p1[1]);
            len=read(p1[0],pipe_buff, sizeof(pipe_buff));
            close(p1[0]);
            for(int i=0; i<files.size(); i++)
            {
                pFile = fopen( files.at(i).c_str(),"wb+" );
                fwrite(pipe_buff,1,len,pFile);
                fclose(pFile);
            }
        }
    }

}

