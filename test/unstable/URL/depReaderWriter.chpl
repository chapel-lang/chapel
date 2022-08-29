use URL;

var style = defaultIOStyleInternal();

const host = "127.0.0.1";
const port = "8000";

var f = "test.txt";
var url = "http://" + host + ":" + port + "/" + f;
var urlreader = openUrlReader(url, style=style);
var urlwriter = openUrlWriter(url, style=style);

