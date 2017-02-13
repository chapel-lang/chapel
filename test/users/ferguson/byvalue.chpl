class SomeData {
  var value:int;
  proc SomeData() {
    writeln("Initializing a SomeData");
    value = 0;
  }
  proc SomeData(x:SomeData) {
    writeln("Initializing a SomeData - copying");
    value = x.value;
  }
  proc deinit() {
    writeln("Destroying a SomeData");
  }
}

record R {
  var data = new SomeData();

  proc deinit() {
    writeln("In ~R() ", data.value);
    delete data;
  }
}


proc =(ret:R, x:R) {
  delete ret.data;
  ret.data = nil;
  ret.data = new SomeData(x.data);
  return ret;
}

proc R.routine() {
  writeln("In R.routine ", data);
}

writeln("Making global");
var globalR:R;
writeln("Done making global");

proc test_two(x:R, setglobal:bool) {
  writeln("In test_two ", x.data);
  if setglobal then globalR = x;
}


proc test_one(x:R, setglobal:bool) {
  writeln("Starting test_one ", x.data);
  x.routine();
  test_two(x, setglobal);
  writeln("Done test_one ", x.data);
}

writeln("Before scope");
{
  var r:R;
  r.data.value = 15;

  test_one(r, false);
}
writeln("After scope");

writeln("Before scope (copy initialization)");
{
  var r:R;
  r.data.value = 15;

  var x:R = r;

  test_one(x, false);
}
writeln("After scope (copy initialization)");

/*
writeln("Before scope (copy initialization 2)");
{
  var r:R;
  r.data.value = 15;

  var x = r;

  test_one(x, false);
}
writeln("After scope (copy initialization 2)");
*/

writeln("Before scope (setting global)");
{
  var r:R;
  r.data.value = 15;

  test_one(r, true);
}
writeln("After scope (setting global)");

writeln("Before scope (begin)");
sync {
  var r:R;
  r.data.value = 15;
  begin {
    test_one(r, false);
  }
}
writeln("After scope (begin)");

writeln("Finishing");
