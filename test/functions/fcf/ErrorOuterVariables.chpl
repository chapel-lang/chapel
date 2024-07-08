//
// Cannot create closures yet.
//

proc test() {
  var x = 4;
  var y = 8;

  const f = proc() {
    writeln(x, y);
    var z = 16;
    const g = proc() { writeln(x, y, z); };
  };

  proc call(fn) { fn(); }
  call(f);
}
test();

