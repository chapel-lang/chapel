use RunServer;
use URL;

config const host = "127.0.0.1";
config const port = "8000";

proc main() throws {
  startServer(host, port);
  defer stopServer();

  var url = "http://" + host + ":" + port + "/test.txt";

  var urlreaderCheck1 = openUrlReader(url, region=..2);
  var str:string;
  urlreaderCheck1.readLine(str);
  urlreaderCheck1.close();
  writeln(str);

  // Note: ideally want to test setting a different starting location, but
  // that requires having an http server that is reliably seekable and I'm not
  // sure how to do that right now.  So for now, explicitly specify what it
  // should already be using
  var urlreaderCheck2 = openUrlReader(url, region=0..);
  urlreaderCheck2.readLine(str);
  urlreaderCheck2.close();
  writeln(str);

  var urlreaderCheck3 = openUrlReader(url, region=0..2);
  urlreaderCheck3.readLine(str);
  urlreaderCheck3.close();
  writeln(str);
}
