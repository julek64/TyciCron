#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sig_handler(int signo);
void sig_int();
void sig_usr1();
void sig_usr2();

void sig_catch()
{
    if (signal(SIGUSR1, sig_handler) == SIG_ERR)
        printf("Error handling SIGUSR1\n");
    if (signal(SIGUSR2, sig_handler) == SIG_ERR)
        printf("Error handling SIGUSR2");
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("Error handling SIGINT\n");
}

void sig_handler(int signo)
{
    if (signo == SIGUSR1)
        sig_usr1();
    else if (signo == SIGUSR2)
        sig_usr2();
    else if (signo == SIGINT)
        sig_int();
}


void sig_int()
{
    printf("\nExecuting SIGINT\n");
    // implementation
    exit(EXIT_SUCCESS); // nie wiem czy to powinno byc exit(), no ale jest
}

void sig_usr1()
{
    printf("\nExecuting SIGUSR1\n");
    // implementation
}

void sig_usr2()
{
    printf("\nExecuting SIGUSR2\n");
    // implementation
}