use Python, IO;

var interp = new Interpreter();


var x = new Value(interp,
            interp.loadPickle(openReader("x.pkl").readAll(bytes)));
writeln(x);
