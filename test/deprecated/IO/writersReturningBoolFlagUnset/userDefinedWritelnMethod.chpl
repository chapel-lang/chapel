// test that user-defined writer methods emit the non-specialized error message

class myclass {
    var x: real;

    proc writeln() {
        writef("%.5\n", this.x);
    }
}

var c = new myclass(5);
if c.writeln() {
    // do something
}
