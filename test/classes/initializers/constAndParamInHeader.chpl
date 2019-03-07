

class Foo {
    var value : int;
    const SOME_CONST : uint = 100;
    proc init(val = SOME_CONST) {
        value = val;
    }
}

class Bar {
    var value : int;
    param SOME_CONST : uint = 100;
    proc init(val = SOME_CONST) {
        value = val;
    }
}

var objA = new shared Foo();
var objB = new shared Bar();

writeln(objA.value);
writeln(objB.value);


