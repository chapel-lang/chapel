
use IO;
var f: file; // make f.reader() throw
forall i in 1..100 with (var reader = try! f.reader()) do ;
