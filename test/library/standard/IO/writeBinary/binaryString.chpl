use IO;

var w = openwriter("./bs.bin");
var d : string = "some very interesting output data " + codepointToString(0x24) + "\n";

w.writeBinary(d);
