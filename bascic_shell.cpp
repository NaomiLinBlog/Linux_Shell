#include <iostream>
#include <unistd.h>
#include <stdlib.h>
using namespace std;
int main(){
    // 從呼叫 fork 開始, 會分成兩支程序多工進行
    pid_t PID = fork();

    switch(PID){
        // PID == -1 fork failed
        case -1:
            perror("fork()");
            exit(-1);

        // PID == 0 代表child process
        case 0:
            cout << "This is Child process. " << endl;
            cout << "Child's PID is "<< getpid() << ". And my return value is "<< PID << endl ;
            break;

        // PID > 0 代表parent process
        default:
            cout << "This is Parent process. " << endl;
            cout << "Parent's PID is "<< getpid() << ". And my return value is "<< PID  << endl;
    }

    return 0;
}
