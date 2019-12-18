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

config const throwOnStep = 2;

proc maybeThrows(x:int) throws {
  if x == throwOnStep then
    throw new owned Error();
}

proc create_thing() {
  var success = false;
  try {
    writeln("creating Container");
    var container = new unmanaged Container();
    defer {
      writeln("in defer for Container");
      if !success then delete container;
    }

    maybeThrows(1);
    writeln("creating Thing 100");
    container.a = new unmanaged Thing(100);
    defer {
      writeln("in defer for Thing 100");
      if !success then delete container.a;
    }

    maybeThrows(2);
    writeln("creating Thing 200");
    container.b = new unmanaged Thing(200);
    defer {
      writeln("in defer for Thing 200");
      if !success then delete container.b;
    }

    maybeThrows(3);
    // success!
    success = true;
    return container:class?;
  } catch {
    return nil;
  }
}

var thing = create_thing();
if thing {
  writeln(thing);
  delete thing!.a;
  delete thing!.b;
  delete thing;
}
