class Thing {
  var x: int;
  proc deinit() {
    writeln("in Thing.deinit, x=", x);
  }

}
class Container {
  var a: unmanaged Thing?;
  var b: unmanaged Thing?;
  proc deinit() {
    writeln("in Container.deinit");
  }
}


config const failOnStep = 0;

proc shouldFail(x:int) {
  return x == failOnStep;
}

proc create_thing() : unmanaged Container? {
  var success = false;
  var ret:unmanaged Container? = nil;

  writeln("creating Container");
  ret = new unmanaged Container();
  defer { if !success then delete ret; }

  if !shouldFail(1) {

    writeln("creating Thing 100");
    ret!.a = new unmanaged Thing(100);
    defer { if !success then delete ret!.a; }

    if !shouldFail(2) {

      writeln("creating Thing 200");
      ret!.b = new unmanaged Thing(200);
      defer { if !success then delete ret!.b; }

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
  delete thing!.a;
  delete thing!.b;
  delete thing;
}
