use URL;
var urlreader = openUrlReader("http://example.com");
var str:string;
// Output each line read from the URL to stdout
while(urlreader.readline(str)) {
 write(str);
}
