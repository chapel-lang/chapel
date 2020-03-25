
var destroyCounter : atomic int;

class Helper {
  var x : 3*int;
}

record Foo {
  var h : unmanaged Helper;
  var serialized = false;

  proc chpl__serialize() {
    return h.x;
  }

  proc type chpl__deserialize(data) {
    return new Foo(new unmanaged Helper(data), true);
  }

  proc deinit() {
    if serialized then destroyCounter.add(1);
    else {
      const numDestroyed = destroyCounter.read();
      assert(numDestroyed == numLocales-1);
    }
    delete h;
  }
}

const f = new Foo(new unmanaged Helper((1,2,3)));

proc main() {
  on Locales.tail() {
    if f.h.x(1) > 100 then halt();
  }
}
