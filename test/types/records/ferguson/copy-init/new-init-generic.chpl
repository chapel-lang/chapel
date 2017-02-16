record R {
  var x;
}


proc R.init(x)
{
  this.x = x;
  super.init();
}

proc R.init(from)
{
  writeln("In R.init");
  this.x = from.x;
  super.init();
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
