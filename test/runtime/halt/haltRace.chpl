// Concurrent halts with a long message used to hang under valgrind (#26973).
config const msgLen = 10_000;

const msg = "x" * msgLen;
var arrived: atomic int;

coforall 0..1 {
  arrived.add(1);
  arrived.waitFor(2);
  halt(msg);
}
