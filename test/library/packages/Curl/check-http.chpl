use RunServer;
use Curl;
use FileSystem;

config const verbose = false;

proc runtest() {
  writeln("checking served files match");

  for f in findfiles() {
    if f.endsWith(".txt") ||
       f.endsWith(".htm") || f.endsWith(".html") ||
       f.endsWith(".chpl") {

      if verbose then
        writeln("Testing with file ", f);

      var filereader = open(f, iomode.r).reader();

      var url = "http://" + host + ":" + port + "/" + f;
      var urlreader = openurl(url).reader();

      var nlines = 0;
      var str1: string;
      var str2: string;
      while true {
        var got1 = filereader.readline(str1);
        var got2 = urlreader.readline(str2);
        if got1 == false && got2 == false then
          break;
        if got1 != got2 then
          halt("file lengths don't match");
        if str1 != str2 then
          halt("file data doesn't match");

        nlines += 1;
      }

      if verbose then
        writeln("Read ", nlines, " lines");
    }
  }
}

startServer();
runtest();
stopServer();
