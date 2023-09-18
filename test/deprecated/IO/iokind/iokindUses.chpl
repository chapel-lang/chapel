use IO;
use Subprocess;

var k : iokind;

var a = iokind.dynamic;
var b = iokind.native;
var c = iokind.little;
var d = iokind.big;

var w = iodynamic;
var x = ionative;
var y = iolittle;
var z = iobig;

var reader : fileReader(false, kind=iokind.native);
var writer : fileWriter(false, kind=iokind.native);
var r2 : fileReader(iokind.native, false);
var w2 : fileWriter(iokind.native, false);

var sub : subprocess(false, kind=iokind.native);
var sub2 : subprocess(iokind.native, false);

writeln(reader.kind);
writeln(writer.kind);
writeln(sub.kind);
