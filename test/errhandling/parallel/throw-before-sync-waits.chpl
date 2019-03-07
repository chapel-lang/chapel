use Time;
use ExampleErrors;

config const taskWait = 2;
config const syncWait = 0;

proc throwingFn() throws {
  throw new owned StringError("test error");
}

var s$:sync int;

proc otherTask() {
  writeln("Starting otherTask");
  sleep(taskWait);
  writeln("Finishing otherTask");
}

proc test() {

  try {
    sync {
      begin otherTask(); 
      sleep(syncWait);
      throwingFn();
    }
    writeln("After sync");
  } catch e {
    writeln("Caught error ", e);
  }
}

test();
