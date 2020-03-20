use IO;
use Help;
use Spawn;
use Random;
use ZMQ;

enum ExecMode {
  Launcher,
  Master,
  Worker
};

config const mode = ExecMode.Launcher;
config const numWorkers = 4;
config const zipcode = 12345;

proc main(args: [] string) {
  if (args.size >= 2) && (args[1] == "--help" || args[1] == "-h") {
    printUsage();
    exit(0);
  }

  select mode {
    when ExecMode.Launcher do Launcher(args[0]);
    when ExecMode.Master   do Master(args[0]);
    when ExecMode.Worker   do Worker(args[0]);
  }
}

const env = [
  "QTHREAD_NUM_SHEPHERDS=1",
  "QTHREAD_NUM_WORKERS_PER_SHEPHERD=1"
  ];

iter zipcodes(num: int) {
  var zips: [0..4] int = [10001 /* New York         */,
                          90001 /* Los Angeles      */,
                          60290 /* Chicago          */,
                          20001 /* Washington, D.C. */,
                          94101 /* San Francisco    */];
  for i in 0..#num do
    yield zips[i % zips.size];
}

proc Launcher(exec: string) {
  var master = spawn(["master", "--mode=Master",
                      "--memLeaks=" + memLeaks:string],
                     env=env, executable=exec);

  var workers: [1..numWorkers] subprocess(kind=iokind.dynamic, locking=true);
  coforall (worker,i,zipc) in zip(workers, workers.domain,
                                  zipcodes(numWorkers)) do
    worker = spawn(["worker%i".format(i), "--mode=Worker",
                    "--memLeaks=" + memLeaks:string,
                    "--zipcode=%i".format(zipc)],
                   env=env, executable=exec);

  for worker in workers do
    worker.communicate();
  master.terminate();
}

proc Master(exec: string) {
  var rand = new borrowed RandomStream(real,13); rand.getNext();
  var ctxt: Context;
  var sock = ctxt.socket(ZMQ.PUB);
  sock.bind("tcp://*:5556");

  record WeatherData {
    var zipcode, temperature, humidity: int;
  }

  while true {
    var data = new WeatherData(
      (rand.getNext()*100000):int,
      (rand.getNext()*215):int - 80,
      (rand.getNext()*50):int + 10);
    sock.send(data);
  }
}

proc Worker(exec: string) {
  const N = 5;

  var ctxt: Context;
  var sock = ctxt.socket(ZMQ.SUB);
  sock.connect("tcp://localhost:5556");
  sock.setSubscribe(zipcode);

  record WeatherData {
    var zipcode, temperature, humidity: int;
  }

  var total = 0;
  for 1..N {
    var data = sock.recv(WeatherData);
    total += data.temperature;
  }
  writef("Average temperature for zipcode '%05i' was %r F\n",
         zipcode, total:real / N);
}
