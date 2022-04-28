#include <iostream>
#include <unistd.h>
#include <vector>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread> 
#include <sstream>
#include <stdio.h>

// void child_status_check(pid_t a){
//     while(true){
//     int b;
//     pid_t result = waitpid(a, &b, WNOHANG);


//     if (result != 0 && result != -1){
//         exit(1);
//     }
//     }
// }



int main (int argc, char **argv) {
    std::vector<pid_t> kids;

    //generate the pipes among the three files (rgen, a1 & a2)
    int rgen_to_a1[2];
    int a1_to_a2[2];
    pipe(rgen_to_a1);
    pipe(a1_to_a2);
    pid_t child_pid;
    
    child_pid = fork(); // child for rgen.cpp
    if(child_pid == 0)
    {
        
        dup2(rgen_to_a1[1], STDOUT_FILENO);
        close(rgen_to_a1[0]);
        close(rgen_to_a1[1]);

        int rgen = execv("./rgen",argv);
        return rgen;
        // return 0;
    }

    kids.push_back(child_pid);
    
    child_pid = fork(); // child for running ece650-a1.py
    if(child_pid == 0)
    {
        dup2(rgen_to_a1[0], STDIN_FILENO); // takes command from rgen
        close(rgen_to_a1[0]);
        close(rgen_to_a1[1]);

        dup2(a1_to_a2[1], STDOUT_FILENO); // sends command to ece650-a2.cpp
        close(a1_to_a2[0]);
        close(a1_to_a2[1]);
        
        char *argv1[3];

        argv1[0] = (char *)"python3";
        argv1[1] = (char *)"./run/bin/ece650-a1.py";
        argv1[2] = nullptr;
        return execvp("python3", argv1);
    }
    kids.push_back(child_pid);

    child_pid = fork(); // child for running ece650-a2.cpp
    if(child_pid == 0)
    {
        dup2(a1_to_a2[0], STDIN_FILENO); // takes command to ece650-a1.cpp
        close(a1_to_a2[0]);
        close(a1_to_a2[1]);

        int assignment_2 = execv("./ece650-a2", argv);
        return assignment_2;
    }
    kids.push_back(child_pid);




    dup2(a1_to_a2[1], STDOUT_FILENO); // prints from ece650-a2.cpp
    close(a1_to_a2[0]);
    close(a1_to_a2[1]);

    
    while (!std::cin.eof()) { // for taking the commands from the keyboard
        sleep(1);
        int status_of_a2;
        pid_t result = waitpid(kids[0], &status_of_a2, WNOHANG);

        if (result != 0 && result != -1){
            break;
        }

        std::string line;
        std::getline(std::cin, line);
        std::cout << line << std::endl;

    }

    for(pid_t k : kids) // loop for closing all the children when eof is generated
    {
        kill(k, SIGTERM);
    }
    return 0;
}
