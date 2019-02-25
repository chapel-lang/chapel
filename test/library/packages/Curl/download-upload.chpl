use RunServer;
use Curl;

config const verbose = false;
config const outUrl = "ftp://127.0.0.1/upload/";

proc runtest() {
  writeln("uploading some files to FTP");

  for f in findfiles() {
    if f.endsWith(".txt") || f.endsWith(".htm") || f.endsWith(".html") {

      if verbose then
        writeln("Testing with file ", f);

      // Open a URL reader and writer
      var inUrl = "http://" + host + ":" + port + "/" + f;
      var outUrlFile = outUrl + f;
      var input = openurl(inUrl).reader();
      var output = openurl(outUrlFile, iomode.cw).writer();

      var str:string;

      var nlines = 0;
      while(input.readline(str)) {
        output.write(str);
        nlines += 1;
      }
      input.close();
      output.close();

      if verbose then
        writeln("Copied ", nlines, " lines");

      // Now, try downloading the file and check against the local file.
      var filereader = open(f, iomode.r).reader();
      var urlreader = openurl(outUrlFile).reader();
      // Now check that the files match
      nlines = 0;
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
        writeln("Verified ", nlines, " lines");
    }
  }
}

startServer();
runtest();
stopServer();

/*
r
// Open a URL and get a reader on a section of the site
config const inUrl = "http://127.0.0.1:8000/2701-h.htm";
var input = openurl(inUrl).reader();

// Open a URL to write to
config const outUrl = "ftp://127.0.0.1/upload/test.txt";
var output = openurl(outUrl, iomode.cw).writer();

var str:string;

var nlines = 0;
// While we can read a line from example.com, write it to 'out.txt'
while(input.readline(str)) {
  output.write(str);
  nlines += 1;
}

writeln("FOUND ", nlines, " lines");
 */
