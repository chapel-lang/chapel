

class Impl {
  var field: int;
  proc init(arg:int) {
    field = arg;
    writeln("in Impl.init ", field);
  }

  proc deinit() {
    writeln("in Impl.~Impl ", field);
  }
}

var globalSharedObject:shared Impl?;

proc makeGlobalSharedObject() {
  var x = new shared Impl(1);

  globalSharedObject = x;
}

makeGlobalSharedObject();

writeln(globalSharedObject!.field);
