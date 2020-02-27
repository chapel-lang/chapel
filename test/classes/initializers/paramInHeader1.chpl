class Bar {
    var value : int;
    param SOME_CONST : uint = 100;
    proc init(val = SOME_CONST) {
        value = val;
    }
}

var objB = new shared Bar();

writeln(objB.value);
