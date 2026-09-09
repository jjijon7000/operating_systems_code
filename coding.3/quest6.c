#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    pid_t first = fork();
    if (first < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (first == 0) {
        sleep(1);
        _exit(10);
    }

    pid_t second = fork();
    if (second < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (second == 0) {
        _exit(20);
    }

    int status;
    if (waitpid(first, &status, 0) < 0) {
        perror("waitpid");
        return EXIT_FAILURE;
    }
    printf("waitpid selected child %ld and received status %d\n",
           (long)first, WEXITSTATUS(status));

    if (waitpid(second, &status, 0) < 0) {
        perror("waitpid");
        return EXIT_FAILURE;
    }
    printf("then collected child %ld with status %d\n",
           (long)second, WEXITSTATUS(status));
    return 0;
}

/*********************************************
    Your name: julio
    Question 6: waitpid() is useful when a parent has multiple children and
    needs to wait for one specific PID. it also supports options such as
    WNOHANG.
*********************************************/