#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid_t writer = fork();
    if (writer < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (writer == 0) {
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) < 0) {
            perror("dup2");
            _exit(EXIT_FAILURE);
        }
        close(pipefd[1]);
        dprintf(STDOUT_FILENO, "message from the first child\n");
        _exit(EXIT_SUCCESS);
    }

    pid_t reader = fork();
    if (reader < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (reader == 0) {
        close(pipefd[1]);
        if (dup2(pipefd[0], STDIN_FILENO) < 0) {
            perror("dup2");
            _exit(EXIT_FAILURE);
        }
        close(pipefd[0]);
        execlp("wc", "wc", "-c", (char *)NULL);
        perror("execlp");
        _exit(EXIT_FAILURE);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(writer, NULL, 0);
    waitpid(reader, NULL, 0);
    return 0;
}

/*********************************************
    Your name: julio
    Question 8: pipe() creates a read end and a write end. the first child
    sends its output to the pipe, and the second child reads from it. unused
    ends must be closed so the reader can detect the end of the data.
*********************************************/