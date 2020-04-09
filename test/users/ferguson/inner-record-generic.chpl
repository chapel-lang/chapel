proc run(x) {

  record R {
    var x:int;
  }

  var r = new R(1);
  writeln( x, " ", r );
}

run(1);
run("hello");
