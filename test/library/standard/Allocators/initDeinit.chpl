use Allocators;

class C {
  proc init() {
    writeln("C.init");
  }
  proc deinit() {
    writeln("C.deinit");
  }
}

{
  var pool = new mallocWrapper();
  var c = newWithAllocator(pool, unmanaged C);
  deleteWithAllocator(pool, c);
}
