use RunServer;
use Curl;

config const verbose = false;
config const url = "ftp://127.0.0.1/upload/test.txt";
config const msg = "this is a test";

proc runtest() {
  writeln("uploading to FTP");

  var output = openurl(url, iomode.cw).writer();

  output.write(msg);
  output.close();
}
runtest();
