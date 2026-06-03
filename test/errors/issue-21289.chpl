module foo {
    record bar_t{
        var a: int;
    }
    proc bar(x: int){
        return x;
    }
}

foo.bar.t;
foo.bar_t;
