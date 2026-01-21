module foo {
    record bar_t{
        var a: int;
    }
    record R {
      var t = 42;
    }
    var bar: R;
}

writeln(foo.bar_t:string);
// Note this is incorrect and should be written as above;
writeln(foo.bar.t);
