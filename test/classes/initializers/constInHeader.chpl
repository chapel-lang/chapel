class Foo {
    var value : int;
    const SOME_CONST : uint = 100;
    proc init(val = SOME_CONST) {
        value = val;
    }
}

var objA = new shared Foo();

writeln(objA.value);
