class Foo {
    var value : int;
    const SOME_CONST : uint = 100; 
    proc Foo(val = SOME_CONST) {
        value = val;
    }
}

class Bar {
    var value : int;
    param SOME_CONST : uint = 100; 
    proc Bar(val = SOME_CONST) {
        value = val;
    }
}

var objA = new borrowed Foo();
var objB = new borrowed Bar();

writeln(objA.value);
writeln(objB.value);
