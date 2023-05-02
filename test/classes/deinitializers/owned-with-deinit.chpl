// modified from https://github.com/chapel-lang/chapel/issues/12973
// Basically a singly linked list.
class A {
  var x: int;
  var next: owned A?;

  proc deinit() {
    while next {
      next = next!.next;
    }
  }
}

proc test(type T) {
  var x = new T(1);
  var y = new T(2, x);
  var z = new T(3, y);
  writeln(z);
}

test(owned A);
