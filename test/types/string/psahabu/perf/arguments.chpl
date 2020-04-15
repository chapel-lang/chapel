use Time;

config const n = 100000000;
config const timing = true;

var pass = "passing a string";
var acc = 0;

test();

// passing
proc testPassing():Timer {
  var tPassing: Timer;
  tPassing.start();
  for i in 1..n {
    acc += receive(pass);
  }
  tPassing.stop();
  return tPassing;
}

// returning
proc testReturning():Timer {
  var tReturning: Timer;
  tReturning.start();
  var keepAlive: int;
  for i in 1..n {
    var s = send(i);
    keepAlive += s.buffLen;
  }
  tReturning.stop();
  return tReturning;
}
proc testReturningItoa():Timer {
  var tReturning: Timer;
  tReturning.start();
  var keepAlive: int;
  for i in 1..(n/10) {
    var s = send_itoa(i);
    keepAlive += s.buffLen;
  }
  tReturning.stop();
  return tReturning;
}


proc test() {
  var tPassing = testPassing();
  var tReturning = testReturning();
  var tReturningItoa = testReturningItoa();

  if timing {
    writeln("passing: ", tPassing.elapsed());
    writeln("returning: ", tReturning.elapsed());
    writeln("returning itoa: ", tReturningItoa.elapsed());
  }

  // acc set in testPassing
  if acc == n * pass.buffLen then
    writeln("SUCCESS");
}


proc receive(test: string) {
  return test.buffLen;
}

proc send(l: int): string {
  var m = l % 3;
  if m == 0 {
    return "returning";
  } else if m == 1 {
    return "a";
  } else {
    return "string";
  }
}

proc send_itoa(l: int): string {
  return l:string;
}
