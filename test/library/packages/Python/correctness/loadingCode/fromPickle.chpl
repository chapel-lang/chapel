use Python, IO;

var interp = new Interpreter();


// must manually hold the GIL, since `loadPickle` is an internal function
var g = new GIL();
var x = new Value(interp,
            interp.loadPickle(openReader("x.pkl").readAll(bytes)));
g.release();
writeln(x);
