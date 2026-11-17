use CTypes;
use Subprocess;

config const injectAgain = false;

extern {
  #include <errno.h>
  #include <sys/select.h>

  static int pending_select_error = 0;
  static int injected_select_errors = 0;

  static void injectSelectError(int again) {
    pending_select_error = again ? EAGAIN : EINTR;
  }

  static int injectedSelectErrors(void) {
    return injected_select_errors;
  }

  int __real_select(int nfds, fd_set* readfds, fd_set* writefds,
                    fd_set* exceptfds, struct timeval* timeout);
  int __wrap_select(int nfds, fd_set* readfds, fd_set* writefds,
                    fd_set* exceptfds, struct timeval* timeout);

  int __wrap_select(int nfds, fd_set* readfds, fd_set* writefds,
                    fd_set* exceptfds, struct timeval* timeout) {
    if (pending_select_error != 0) {
      errno = pending_select_error;
      pending_select_error = 0;
      injected_select_errors++;
      return -1;
    }
    return __real_select(nfds, readfds, writefds, exceptfds, timeout);
  }
}

var process = spawn(["cat"], stdin=pipeStyle.bufferAll,
                    stdout=pipeStyle.pipe, stderr=pipeStyle.pipe);
process.stdin.writeln("retry succeeded");
injectSelectError(injectAgain:c_int);
process.communicate();
assert(injectedSelectErrors() == 1);
assert(process.exitCode == 0);
assert(process.stdout.readAll(string) == "retry succeeded\n");
var errorOutput: string;
assert(!process.stderr.readLine(errorOutput));
process.close();
writeln("OK");
