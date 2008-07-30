class Foo {
    var value : int;
    const SOME_CONST : uint = 100; 
    def Foo(val = SOME_CONST) {
        value = val;
    }
}

class Bar {
    var value : int;
    param SOME_CONST : uint = 100; 
    def Bar(val = SOME_CONST) {
        value = val;
    }
}

var objA = new Foo();
var objB = new Bar();

writeln(objA.value);
writeln(objB.value);


