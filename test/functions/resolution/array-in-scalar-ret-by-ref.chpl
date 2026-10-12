class C {
    var arr: [1..10] int;

    proc ref getArrayElt() ref : int {
        return arr;
    }
}

var c = new C();
var x = c.getArrayElt();
