use URL;

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
