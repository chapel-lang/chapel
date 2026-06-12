module foo {
  record R {
    var t = 42;
  }

    record bar_t{
        var a: int;
    }
    proc bar {
      return new R();
    }
}

writeln(foo.bar_t:string);
// Note this is incorrect and should be written as above;
writeln(foo.bar.t);
