#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    int x = 100;
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        x = 200;
        printf("child: x started at 100 and is now %d\n", x);
        return 0;
    }

    x = 300;
    printf("parent: x started at 100 and is now %d\n", x);
    if (waitpid(pid, NULL, 0) < 0) {
        perror("waitpid");
        return EXIT_FAILURE;
    }
    return 0;
}

/*********************************************
    Your name: julio
    Question 1: fork() gives the child its own copy of memory. both processes
    see x as 100 at first, but changing x in one process does not change the
    other process's copy.
*********************************************/