
use IO;
var f = open("example-pr-13203-c.chpl", ioMode.r);
forall i in 1..100 with (var reader = try! f.reader()) do ;
