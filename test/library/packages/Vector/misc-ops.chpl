
use Vector;

proc main() {
  // vector.reverse()
  {
    var A, B : vector(int);
    for i in 1..10 {
      A.push_back(i);
      B.push_front(i);
    }
    A.reverse();
    assert(A.equals(B));
  }

  {
    var A : vector(int);
    for i in 1..10 do A.push_back(i);

    A.clear();
    assert(A.isEmpty());

    for i in 1..100 do A.push_back(i);

    A.clear();
    assert(A.isEmpty());
  }

  {
    var A : vector(int);
    for i in 1..100 do A.push_back(i);

    for i in 1..100 by 4 {
      const (found, idx) = A.find(i);
      assert(found == true);
      assert(idx == i-1);
      assert(A[idx] == i);
    }

    const (found, _) = A.find(0);
    assert(found == false);
  }

  // promotion
  {
    proc test(x : int) {
      return x + 1;
    }

    var A, B : vector(int);
    for i in 1..100 {
      A.push_back(i);
      B.push_back(i + 1);
    }

    const C = test(A);
    assert(B.equals(C));

    const D = A + 1;
    assert(B.equals(D));
  }

  // Simple indexing
  {
    var A : vector(int);
    for i in 1..100 do A.push_back(i);

    for i in 0..#A.size do assert(A[i] == i + 1);

    for i in 0..#A.size do A[i] += 1;
    for i in 0..#A.size do assert(A[i] == i + 2);
  }

  // Iteration
  {
    var A : vector(int);
    for i in 1..100 do A.push_back(i);

    var B : [1..100] int = 1..100;

    for (a, b) in zip(A, B) do assert(a == b);
    for (b, a) in zip(B, A) do assert(b == a);

    forall (a, b) in zip(A, B) do assert(a == b);
    forall (b, a) in zip(B, A) do assert(b == a);
  }

  {
    var A : vector(int);
    A.requestCapacity(100);
    assert(A._dom.size >= 100);
  }
}
