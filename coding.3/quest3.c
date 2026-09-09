#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    int ready[2];
    if (pipe(ready) < 0) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        close(ready[0]);
        printf("hello\n");
        fflush(stdout);
        if (write(ready[1], "x", 1) != 1) {
            perror("write");
            _exit(EXIT_FAILURE);
        }
        close(ready[1]);
        _exit(EXIT_SUCCESS);
    }

    close(ready[1]);
    char signal;
    if (read(ready[0], &signal, 1) != 1) {
        perror("read");
        close(ready[0]);
        return EXIT_FAILURE;
    }
    close(ready[0]);
    printf("goodbye\n");
    return 0;
}

/*********************************************
    Your name: julio
    Question 3: a pipe synchronizes the processes without wait(). the child
    prints hello and sends one byte. the parent waits for that byte, then
    prints goodbye. normally, the parent should also reap the child.
*********************************************/