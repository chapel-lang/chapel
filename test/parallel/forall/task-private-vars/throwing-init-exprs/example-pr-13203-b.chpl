
use IO;
var f = open("", iomode.r);
forall i in 1..100 with (var reader = try! f.reader()) do ;
