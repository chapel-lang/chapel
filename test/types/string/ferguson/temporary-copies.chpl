use Time;


config const timing = false;
config const n = 3;
config const m = 100;

var globalString = "";

var tg:Timer;
var t0:Timer;
var t1:Timer;
var t2:Timer;
var t3:Timer;
var t4:Timer;

tg.start();

for i in 1..n {
  globalString += (i%10):string;
}

tg.stop();

var ones = 0;

// 0 temporaries
t0.start();
for i in 1..m {
  ones += globalString.count("1");
}
t0.stop();

// 1 temporary
t1.start();
for i in 1..m {
  var a = globalString;
  ones += a.count("1");
}
t1.stop();

// 2 temporaries
t2.start();
for i in 1..m {
  var a = globalString;
  var b = a;
  ones += b.count("1");
}
t2.stop();

// 3 temporaries
t3.start();
for i in 1..m {
  var a = globalString;
  var b = a;
  var c = b;
  ones += c.count("1");
}
t3.stop();

// 4 temporaries
t4.start();
for i in 1..m {
  var a = globalString;
  var b = a;
  var c = b;
  var d = c;
  ones += d.count("1");
}
t4.stop();

ones /= m;
ones /= 5; // 5 different loops

writeln("Result: ", ones);

if timing {
  writeln("time in seconds to construct an ", n, " byte string");
  writeln("s build          ", tg.elapsed());
  writeln("time in seconds to for ", m, " iterations");
  writeln("0 temporaries    ", t0.elapsed());
  writeln("1 temporaries    ", t1.elapsed());
  writeln("2 temporaries    ", t2.elapsed());
  writeln("3 temporaries    ", t3.elapsed());
  writeln("4 temporaries    ", t4.elapsed());
}

