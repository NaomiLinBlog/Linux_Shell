
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int x = 1;
int pfd[2];

void signal_handler(int sig)
{
    char buffer;
    read(pfd[0], &buffer, 1);
    x = buffer;
    std::cout << "child:  have received a number: " << (int)buffer << std::endl;
}

int main()
{
    pipe(pfd);
    pid_t pid = fork();
    if (pid == 0) {
        signal(SIGUSR1, signal_handler);
        for (int count = 1; ; count++) {
            if (count % x == 0) {
                std::cout << "child:  " << count << " seconds have passed." << std::endl << std::flush;
            }
            sleep(1);
        }
    } else {
        int status;
        while(true) {
            int input;
            std::cout << "Please input a number: " << std::endl;
            std::cin >> input;
            char buffer = input;
            write(pfd[1], &buffer, 1);
            kill(pid, SIGUSR1);
            std::cout << "parent: have sent a SIGUSR1 signal to child(PID = " << (int)pid << ")" << std::endl;
        }
        waitpid((pid_t)pid, &status, 0);
        std::cout << status << std::endl;
    }

    return 0;
}

