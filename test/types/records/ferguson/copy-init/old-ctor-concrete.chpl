record R {
  var x:int;
}


proc R.R(from:R)
{
  writeln("In R.ctor");
  this.x = from.x;
}


proc run() {
  writeln("Creating r");
  var r = new R(1);
  writeln("r ", r);
  writeln("copy-initializing");
  var copy1 = r;
  writeln("copy1 ", copy1);
  writeln("explicitly copy-initializing");
  var copy2 = new R(r);
  writeln("copy2 ", copy2);
  writeln("r ", r);
}

run();
