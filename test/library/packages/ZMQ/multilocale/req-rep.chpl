use ZMQ;

record Bar {
  var a: complex;
  var b: string;
}

record Foo {
  var a: int;
  var b: real;
  var c: string;
  var d: Bar;
}

enum Baz {
  A, B, C, D, E
}

proc Master() {
  var context: Context;
  var socket = context.socket(ZMQ.REP);
  socket.bind("tcp://*:5556");

  // Numeric Types
  {
    var val = socket.recv(real) * 42.0;
    socket.send(val);
  }

  // Enumerated Types
  {
    var val = socket.recv(Baz);
    assert(val == Baz.B);
    socket.send(Baz.D);
  }

  // Strings
  {
    var val = socket.recv(string);
    val += "jumps over the lazy dog.";
    socket.send(val);
  }

  // "Basic" Records
  {
    var val = socket.recv(Foo);
    val.a *= 2;
    val.b += 29.0;
    val.c = val.c.toUpper();
    val.d.a *= -1.0i;
    val.d.b = val.d.b.toTitle();
    socket.send(val);
  }
}

proc Worker() {
  var context: Context;
  var socket = context.socket(ZMQ.REQ);
  socket.connect("tcp://localhost:5556");

  // Numeric Types
  {
    var val = 13.0;
    socket.send(val);
    val = socket.recv(real);
    writeln("val = ", val);
  }

  // Enumerated Types
  {
    var val = Baz.B;
    socket.send(val);
    val = socket.recv(Baz);
    writeln("val = ", val);
  }

  // Strings
  {
    var val = "The quick brown fox...";
    socket.send(val);
    val = socket.recv(string);
    writeln("val = ", val);
  }

  // "Basic" Records
  {
    var val = new Foo(42, 13.0, "hello", new Bar(4+8i, "good bye"));
    socket.send(val);
    val = socket.recv(Foo);
    writeln("val = ", val);
  }
}

if numLocales < 2 {
  writeln("This test requires -nl or --numLocales to be at least 2!");
  exit(1);
}

cobegin {
  on Locales[0]            do Master();
  on Locales[numLocales-1] do Worker();
}
