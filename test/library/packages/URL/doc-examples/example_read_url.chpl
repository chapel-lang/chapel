//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE */
use URL;
var urlreader = openUrlReader("http://example.com");
var str: bytes;
// Output each line read from the URL to stdout
while urlreader.readLine(str) {
  write(str);
}
/* STOP_EXAMPLE */
