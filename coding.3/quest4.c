#define _GNU_SOURCE

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static void run_exec(const char *variant) {
    char *const args[] = {"ls", "-l", "/bin/ls", NULL};
    char *const environment[] = {"PATH=/usr/bin:/bin", "QUEST4=execle", NULL};

    if (strcmp(variant, "execl") == 0) {
        execl("/bin/ls", "ls", "-l", "/bin/ls", (char *)NULL);
    } else if (strcmp(variant, "execlp") == 0) {
        execlp("ls", "ls", "-l", "/bin/ls", (char *)NULL);
    } else if (strcmp(variant, "execle") == 0) {
        execle("/bin/ls", "ls", "-l", "/bin/ls", (char *)NULL, environment);
    } else if (strcmp(variant, "execv") == 0) {
        execv("/bin/ls", args);
    } else if (strcmp(variant, "execvp") == 0) {
        execvp("ls", args);
    } else if (strcmp(variant, "execvpe") == 0) {
        execvpe("ls", args, environment);
    } else {
        fprintf(stderr, "usage: %s [execl|execlp|execle|execv|execvp|execvpe]\n", "quest4");
        _exit(EXIT_FAILURE);
    }

    fprintf(stderr, "%s failed: %s\n", variant, strerror(errno));
    _exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    const char *variant = argc == 2 ? argv[1] : "execvp";
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (pid == 0) {
        run_exec(variant);
    }
    int status;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return EXIT_FAILURE;
    }
    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAILURE;
}

/*********************************************
    Your name: julio
    Question 4: the exec versions differ by argument format, PATH searching,
    and whether they use a custom environment. a successful exec() replaces
    the child program and does not return.
*********************************************/