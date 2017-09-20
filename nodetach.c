#include <errno.h>
#include <err.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sysexits.h>
#include <spawn.h>

int main(int argc, char *argv[], char *envp[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
    exit(EX_USAGE);
  }

  if (prctl(PR_SET_CHILD_SUBREAPER, /*arg2=*/true, /*arg3=*/0, /*arg4=*/0,
            /*arg5=*/0) != 0) {
    err(EX_OSERR, "prctl");
  }

  pid_t pid = -1;
  if (posix_spawnp(&pid, argv[1], /*file_actions=*/NULL, /*attrp=*/NULL,
                   argv + 1, envp) != 0) {
    err(EX_OSERR, "posix_spawnp");
  }

  int exit_code = EX_OK;
  int wstatus = -1;
  while (true) {
    pid = wait(&wstatus);
    if (pid == -1) {
      if (errno == EINTR) {
        continue;
      } else if (errno == ECHILD) {
        // No more children
        break;
      }
      err(EX_OSERR, "wait");
    }
    if (WIFEXITED(wstatus)) {
      exit_code = WEXITSTATUS(wstatus);
    } else {
      // Child exited due to e.g. signal
      exit_code = 255;
    }
  }

  return exit_code;
}
