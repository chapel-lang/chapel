@functionStatic
var x = 1;

record R {
    @functionStatic
    proc fn1() {}

    proc fn2() {
        @functionStatic
        var x = 2;
    }
}
