
var count: atomic int;

record Count {
  var counter: int;
  proc init() { counter = count.fetchAdd(1); }
  // a non-empty destructor
  proc deinit() { if counter < 0 then writeln("deleting Count ", counter); } 
}

record Wrapper {
  var ptr: unmanaged object?;
  proc init() { ptr = new unmanaged object(); }
  proc init=(arg: Wrapper) { ptr = nil; } // do not allocate a new object
  proc deinit() { delete ptr; }
}

var Global = new Wrapper();

config const numMessages = 12;
config const dptpl = 3;

const MessageSpace = {1..numMessages};

forall msg in MessageSpace.these(tasksPerLocale = dptpl)
  with (var cnt: Count, var itr = cnt.counter * 100)
{
  itr += 1;
  // Like in block.chpl, 'cnt.counter' depends on the structure
  // of the these() iterator and may change. The correct behavior
  // is when we get dptpl different counter values.
  writef("%i %i\n", cnt.counter, itr);
}

forall msg in MessageSpace.these(tasksPerLocale = dptpl)
  with (var wrap: Wrapper,
        // make sure that other ways to declare a TPV
        // are usable, too
        var wrap2: Wrapper = Global,
        var wrap3          = Global)
{
  writeln(if wrap.ptr then "yes" else "no");
}
