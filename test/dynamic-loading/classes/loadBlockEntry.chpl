use BlockEntry;
use CTypes;
use DynamicLibrary;

config const path = "./lib";

proc checkStream(ref lib, name: string, alpha: ?t) {
  var a = new owned Entry(t);
  var b = new owned Entry(t);
  forall i in a.values.domain {
    a.values[i] = i - 3;
    b.values[i] = 2*i + 1;
  }

  const stream = lib.retrieve(name, proc(a: c_ptr(void), b: c_ptr(void),
                                         alpha: t): c_ptr(void));
  const ptr = stream(c_ptrTo(a), c_ptrTo(b), alpha);
  var output = owned.adopt((ptr: unmanaged Entry(t)?)!);
  for i in a.values.domain do
    assert(output.values[i] == (i - 3) + (2*i + 1) * alpha);
}

proc main() {
  var entry = new owned Entry();
  forall i in entry.values.domain do entry.values[i] = i;

  var lib = load("addOneLib", directory=path);
  const addOne = lib.retrieve("addOne", proc(ptr: c_ptr(void)): void);
  addOne(c_ptrTo(entry));
  checkStream(lib, "streamInt", 2);
  checkStream(lib, "streamReal", 0.5);
}
