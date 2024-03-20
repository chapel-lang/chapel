use URL;

var url = "http://example.com";

var urlreaderCheck1 = openUrlReader(url, start=3);
var urlreaderCheck2 = openUrlReader(url, end=10);
var urlreaderCheck3 = openUrlReader(url, start=3, end=10);

var urlwriterCheck1 = openUrlWriter(url, start=3);
var urlwriterCheck2 = openUrlWriter(url, end=10);
var urlwriterCheck3 = openUrlWriter(url, start=3, end=10);
