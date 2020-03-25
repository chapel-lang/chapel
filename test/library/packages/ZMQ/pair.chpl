use Help;
use Spawn;
use ZMQ;

enum ExecMode {
  Launcher,
  Master,
  Worker,
};

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
  A=1, B, C, D, E
}

config const mode = ExecMode.Launcher;

const env = [
  "QTHREAD_NUM_SHEPHERDS=1",
  "QTHREAD_NUM_WORKERS_PER_SHEPHERD=1"
  ];

proc main(args: [] string) {
  if (args.size >= 2) && (args[1] == "--help" || args[1] == "-h") {
    printUsage();
    exit(0);
  }

  select mode {
    when ExecMode.Launcher do Launcher(args[0]);
    when ExecMode.Master   do Master();
    when ExecMode.Worker   do Worker();
  }
}

proc Launcher(exec: string) {
  var master = spawn(["master", "--mode=Master",
                      "--memLeaks=" + memLeaks:string],
                     env=env, executable=exec);
  var worker = spawn(["worker", "--mode=Worker",
                      "--memLeaks=" + memLeaks:string],
                     env=env, executable=exec);
  master.communicate();
  worker.communicate();
}

proc Master() {
  var context = new ZMQ.Context();
  var socket = context.socket(ZMQ.PAIR);
  socket.bind("tcp://*:5555");

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

  // Bytes
  {
    var val = socket.recv(bytes);
    val += b"jumps over the \xff\xff\xff non-UTF8 bytes.";
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
  var context = new ZMQ.Context();
  var socket = context.socket(ZMQ.PAIR);
  socket.connect("tcp://localhost:5555");

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

  // Bytes
  {
    var val = b"The quick \xff\xff\xff brown fox...";
    socket.send(val);
    val = socket.recv(bytes);
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
