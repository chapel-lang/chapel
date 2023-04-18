use WeakPointer;

class C {
    var x: int;
}

var w = new weak(shared C);

try {
    var c = w : shared C;
} catch e: NilClassError {
    writeln("correct");
} catch e {
    writeln("Unexpected error: ", e);
}
