use URL;

// Reader
var url = "http://example.com";

var urlreaderCheck1 = openUrlReader(url, region=..8);
var str:string;
urlreaderCheck1.readLine(str);
urlreaderCheck1.close();
writeln(str);

var urlreaderCheck2 = openUrlReader(url, region=2..);
urlreaderCheck2.readLine(str);
urlreaderCheck2.close();
writeln(str);

var urlreaderCheck3 = openUrlReader(url, region=2..8);
urlreaderCheck3.readLine(str);
urlreaderCheck3.close();
writeln(str);

// Writer
/* const host = "127.0.0.1";
const port = "8000";
var f = "test.txt";
url = "http://" + host + ":" + port + "/" + f;

var urlwriterCheck1 = openUrlWriter(url, region=..8);
urlwriterCheck1.writeln("blablabla");
urlwriterCheck1.close();

var validateWriterCheck1 = openUrlReader(url);
validateWriterCheck1.readLine(str);
validateWriterCheck1.close();
writeln(str);

var urlwriterCheck2 = openUrlWriter(url, region=2..);
urlwriterCheck2.writeln("blablabla");
urlwriterCheck2.close();

var validateWriterCheck2 = openUrlReader(url);
validateWriterCheck2.readLine(str);
validateWriterCheck2.close();
writeln(str);

var urlwriterCheck3 = openUrlWriter(url, region=2..8);
urlwriterCheck3.writeln("blablabla");
urlwriterCheck3.close();

var validateWriterCheck3 = openUrlReader(url);
validateWriterCheck3.readLine(str);
validateWriterCheck3.close();
writeln(str); */
