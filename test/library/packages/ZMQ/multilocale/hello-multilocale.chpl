use ZMQ, IO;

proc Master() {
  var context: Context;
  on Locales[(here.id+1) % numLocales] {
    var socket = context.socket(ZMQ.PUSH);
    on Locales[(here.id+1) % numLocales] {
      socket.bind("tcp://*:5555");
      on Locales[(here.id+1) % numLocales] {
        writeln("Master socket lives on locale ", socket.classRef.home.id);
        var msg = "I'm locale %i".format(here.id);
        socket.send(msg);
      }
    }
  }
}

proc Worker() {
  var context: Context;
  on Locales[(here.id+numLocales-1) % numLocales] {
    var socket = context.socket(ZMQ.PULL);
    on Locales[(here.id+numLocales-1) % numLocales] {
      socket.connect("tcp://localhost:5555");
      on Locales[(here.id+numLocales-1) % numLocales] {
        writeln("Worker socket lives on locale ", socket.classRef.home.id);
        var msg = socket.recv(string);
        writeln("\"Hello, %s\", sees locale %i".format(msg, here.id));
      }
    }
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
