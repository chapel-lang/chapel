class Thing {
  var x: int;
  proc deinit() {
    writeln("in Thing.deinit, x=", x);
  }

}
class Container {
  var a: Thing;
  var b: Thing;
  proc deinit() {
    writeln("in Container.deinit");
  }
}


config const failOnStep = 0;

proc shouldFail(x:int) {
  return x == failOnStep;
}

proc create_thing() : Container {
  var success = false;
  var ret:Container = nil;

  writeln("creating Container");
  ret = new Container();
  defer { if !success then delete ret; }

  if !shouldFail(1) {

    writeln("creating Thing 100");
    ret.a = new Thing(100);
    defer { if !success then delete ret.a; }

    if !shouldFail(2) {

      writeln("creating Thing 200");
      ret.b = new Thing(200);
      defer { if !success then delete ret.b; }

      if !shouldFail(3) {
        // success!
        success = true;
      }
    }
  }

  if success then
    return ret;
  else
    return nil;
}

var thing = create_thing();
if thing {
  writeln(thing);
  delete thing.a;
  delete thing.b;
  delete thing;
}
