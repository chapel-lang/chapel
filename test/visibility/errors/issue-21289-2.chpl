module foo {
    record bar_t{
        var a: int;
    }
    proc bar(x: int){
        return x;
    }
}

use foo;
foo.bar.t;
// Note this is incorrect and should be;
foo.bar_t;
