
use Vector;

proc main() {
  {
    var A : vector(int);
    var B = new vector(int);
    writeln("empty vector: '", A, "'");
    writeln("empty vector: '", B, "'");
    assert(A.size == 0 && B.size == 0);
    assert(A.isEmpty() && B.isEmpty());
  }

  {
    var A = [1, 2, 3, 4, 5];
    var B = new vector(A);
    writeln("init with array: '", B, "'");
    assert(A.size == B.size);
  }

  {
    var A = new vector([1, 2, 3, 4, 5]);
    var B = A;
    writeln("copy init: '", B, "'");
    assert(A.size == B.size && A.size == 5);
  }

  {
    var A = new vector(int, 100);
    writeln("with capacity: '", A, "'");
    writeln("  capacity = ", A._dom.size);
    assert(A.isEmpty());
    assert(A.size == 0);
  }
}
