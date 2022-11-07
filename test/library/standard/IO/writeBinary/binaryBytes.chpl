use IO;

var w = openwriter("./bb.bin");
var d : bytes = b"some very interesting output data \xF0\x9F\x91\x8D\n";

w.writeBinary(d);
