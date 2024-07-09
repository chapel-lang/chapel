use URL;

var url = "http://example.com";

var urlwriterCheck1 = openUrlWriter(url, region=..8);
urlwriterCheck1.close();

var urlwriterCheck2 = openUrlWriter(url, region=2..);
urlwriterCheck2.close();

var urlwriterCheck3 = openUrlWriter(url, region=2..8);
urlwriterCheck3.close();
