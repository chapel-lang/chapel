//
// Start a couple of tasks, but don't let either one finish.
//
var t1_notify$: single int;
var t1_release$: single int;
begin {
  t1_notify$.writeEF(0);
  t1_release$.readFF();
}

var t2_notify$: single int;
var t2_release$: single int;
begin {
  t2_notify$.writeEF(0);
  t2_release$.readFF();
}

//
// Wait for both tasks to be running.
//
t1_notify$.readFF();
t2_notify$.readFF();

//
// Send ourselves a SIGINT, as if someone typed a CTRL-C to us.
// This will cause us to produce a task report and halt.
//
// Note: Should use the OS.POSIX module for all this, but it's
// not ready yet.
//
extern proc getpid(): int(32);
extern proc kill(pid: int(32), sig: int): int;
const ret = kill(getpid(), 2 /*SIGINT*/);
if ret != 0 then writeln('kill(getpid(), 2 /*SIGINT*/) returned ', ret);
