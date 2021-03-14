record R {
  var x:int;
}


proc R.init(x:int)
{
  this.x = x;
}

proc R.init=(from:R)
{
  writeln("In R.init=");
  this.x = from.x;
}

proc run() {
  writeln("Creating r");
  var r = new R(1);
  writeln("r ", r);
  writeln("copy-initializing");
  var copy1 = r;
  writeln("copy1 ", copy1);
  writeln("r ", r);
}

run();
