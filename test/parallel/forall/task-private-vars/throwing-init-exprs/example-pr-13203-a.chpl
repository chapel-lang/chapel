
use IO;
var f = open("", iomode.rw);
forall i in 1..100 with (var reader = f.reader()) do ;
