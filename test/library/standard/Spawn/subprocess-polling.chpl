use Subprocess;
use Time;

config const child = false;
config const quietSeconds = 0.2;
config const trials = 3;
config const concurrentWaits = 1;
config const usePipes = true;
config const report = false;

extern {
  #include <errno.h>
  #include <sys/resource.h>
  #include <time.h>

  static double parentCpuSeconds(void) {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_utime.tv_sec + usage.ru_stime.tv_sec +
           (usage.ru_utime.tv_usec + usage.ru_stime.tv_usec) * 1.0e-6;
  }

  static void quietChild(double seconds) {
    struct timespec remaining;
    remaining.tv_sec = (time_t) seconds;
    remaining.tv_nsec = (long) ((seconds - remaining.tv_sec) * 1.0e9);
    while (nanosleep(&remaining, &remaining) != 0 && errno == EINTR) { }
  }
}

proc main(args: [] string) throws {
  if child {
    quietChild(quietSeconds);
    if usePipes then writeln("child finished");
    return;
  }

  var elapsed: stopwatch;
  const cpuStart = parentCpuSeconds();
  elapsed.start();
  coforall waiter in 1..concurrentWaits do
  for trial in 1..trials {
    const command = [args[0], "--child=true",
                     "--quietSeconds=" + quietSeconds:string,
                     "--usePipes=" + usePipes:string];
    if usePipes {
      var process = spawn(command, stdout=pipeStyle.pipe,
                          stderr=pipeStyle.pipe);
      process.communicate();
      assert(process.exitCode == 0);
      assert(process.stdout.readAll(string) == "child finished\n");
      var errorOutput: string;
      assert(!process.stderr.readLine(errorOutput));
      process.close();
    } else {
      var process = spawn(command);
      process.wait(buffer=false);
      assert(process.exitCode == 0);
    }
  }
  elapsed.stop();
  const cpuSeconds = parentCpuSeconds() - cpuStart;
  if report {
    writeln("wall_seconds=", elapsed.elapsed());
    writeln("parent_cpu_seconds=", cpuSeconds);
    writeln("parent_cpu_fraction=", cpuSeconds / elapsed.elapsed());
  } else {
    writeln("OK");
  }
}
