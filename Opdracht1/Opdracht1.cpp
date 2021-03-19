#include <iostream>
#include <cstring>
#include <unistd.h>
#include <syscall.h>
#include <fcntl.h>
#include <sys/wait.h>
using namespace std;

void new_file(){
    string name;
    string text;
    cout << "Bestandsnaam: ";
    cin >> name;
    cout << "Tekst: ";
    cin.ignore();
    while(true){
        string temp="";
        getline (cin,temp);
        if(temp=="<EOF>"){
            break;
        }else{
            text+=temp;
        }
    }
    const char * name2 = name.c_str();
    const char * text2 = text.c_str();
    int newfile = syscall(SYS_creat, name2, 0755);
    int openfile = syscall(SYS_open, name2, O_CREAT|O_RDWR|O_TRUNC, 0755);
    int write = syscall(SYS_write, openfile, text2, text.size());
    close(newfile);
    close(openfile);
    close(write);
}

void list(){
    pid_t pid = fork();
    char *cmd[]={(char*)"/bin/ls", (char*)"-la", NULL};
    if (pid > 0)
    {
        wait(0);
    }
    else
    {
        execve(cmd[0], cmd, NULL);
    }
}

void find(){
    string woord;
    cout<<"Voer een woord in: ";
    cin>>woord;
    const char* woord2=woord.c_str();
    int pipefd[2];
    pipe(pipefd);
    int pid1 = fork();
    if(pid1 == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        const char* args[]={"/usr/bin/find", ".", NULL};
        syscall(SYS_execve, args[0], args, NULL);
    }
    pid_t pid2 = fork();
    if(pid2 == 0)
    {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        const char* args2[]={"/bin/grep", woord2, NULL};
        syscall(SYS_execve, args2[0], args2, NULL);
    }
    close(pipefd[0]);
    close(pipefd[1]);
    int status;
    wait(&status);
}


void seek(){
    int newfile = syscall(SYS_creat, "loop", 0755);
    int write = syscall(SYS_write, newfile, "x", 1);
    for(unsigned int i=0; i<5000000; i++){
        write = syscall(SYS_write, newfile, "\0", 1);
    }
    write = syscall(SYS_write, newfile, "x", 1);
    close(newfile);
    close(write);

    int newfileseek = syscall(SYS_creat, "seek", 0755);
    int writeseek = syscall(SYS_write, newfileseek, "x", 1);
    int fdpos = syscall(SYS_lseek, newfileseek, 5000000, 1);
    syscall(SYS_write, newfileseek, "x", 1);
    close(newfileseek);
    close(writeseek);
}

void src() // Voorbeeld: Gebruikt SYS_open en SYS_read om de source van de shell (shell.cc) te printen.
{ int pipefd = syscall(SYS_open, "shell.cc", O_RDONLY, 0755); // Gebruik de SYS_open call om een bestand te openen.
    char byte[1];                                           // 0755 zorgt dat het bestand de juiste rechten krijgt (leesbaar is).
    while(syscall(SYS_read, pipefd, byte, 1))                   // Blijf SYS_read herhalen tot het bestand geheel gelezen is,
        std::cout << byte; }                                  //   zet de gelezen byte in "byte" zodat deze geschreven kan worden.


int main()
{ std::string input;
    std::string prompt = "Voer een commando in: ";

    while(true)
    { std::cout << prompt;
        std::getline(std::cin, input);
        if (input == "new_file") new_file();
        else if (input == "ls") list();
        else if (input == "src") src();
        else if (input == "find") find();
        else if (input == "seek") seek();
        else if (input == "exit") return 0;
        else if (input == "quit") return 0;
        else if (input == "error") return 1;

        if (std::cin.eof()) return 0; } }
