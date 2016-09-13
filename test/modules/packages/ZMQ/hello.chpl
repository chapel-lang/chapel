use Help;
use Spawn;
use ZMQ;

enum ExecMode {
  Launcher,
  Master,
  Worker,
};

config const mode = ExecMode.Launcher;
config const to = "world";

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
  var toFlag = "--to=" + to;
  var master = spawn(["master", "--mode=Master",
                      "--memLeaks=" + memLeaks:string, toFlag],
                     env=env, executable=exec);
  var worker = spawn(["worker", "--mode=Worker",
                      "--memLeaks=" + memLeaks:string],
                     env=env, executable=exec);
  master.communicate();
  worker.communicate();
}

proc Master() {
  var context: Context;
  var socket = context.socket(ZMQ.PUSH);
  socket.bind("tcp://*:5555");
  socket.send(to);
}

proc Worker() {
  var context: Context;
  var socket = context.socket(ZMQ.PULL);
  socket.connect("tcp://localhost:5555");
  writeln("Hello, ", socket.recv(string));
}
