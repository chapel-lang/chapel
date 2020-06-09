record r { var x: int = 0; }
proc r.deinit() { writeln('deinit'); }

record cell {
  type T;

  // Simulate stores where memory is not default initialized.
  pragma "no init"
  var _data: T;

  proc _storeHelper(in x: T) {
    _data = x;
  }

  proc store(in x: T) {
    on this {
      _storeHelper(x);
    }
  }
}

proc test() {
  var c = new cell(r);
  var x = new r();

  c.store(x);
}
test();

