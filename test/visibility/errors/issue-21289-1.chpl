module foo {
    record bar_t{
        var a: int;
    }
    proc bar(x: int){
        return x;
    }
}

writeln(foo.bar_t:string);
// Note this is incorrect and should be written as above;
writeln(foo.bar.t);
