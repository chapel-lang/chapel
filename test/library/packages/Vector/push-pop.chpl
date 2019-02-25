
use Vector;

proc main() {
  {
    var A = new vector(int);
    for i in 1..10 {
      A.push_back(i);
      assert(A.back() == i);
    }
    assert(A.size == 10);
    assert(A.front() == 1);
  }
  {
    var A = new vector(int);
    A.push_back([1, 2, 3, 4]);
    assert(A.size == 4 && A.front() == 1 && A.back() == 4);

    var B = new vector(int);
    B.push_back(A);
    assert(B.size == 4 && B.front() == 1 && B.back() == 4);

    B.push_back([5, 6, 7, 8]);

    for (i, b) in zip(B.front()..B.back(), B) do assert(i == b);
  }

  {
    var A = new vector(int);
    for i in 1..10 do A.push_back(i);

    for i in 1..10 by -1 {
      const b = A.back();
      const res = A.pop_back();
      assert(b == res);
      assert(res == i);
      assert(A.size == i - 1);
    }
  }

  {
    var A = new vector(int);
    for i in 1..10 {
      A.push_front(i);
      assert(A.front() == i);
    }
    assert(A.size == 10);
    assert(A.back() == 1);
  }
  {
    var A = new vector(int);
    A.push_front([1, 2, 3, 4]);
    assert(A.size == 4 && A.front() == 1 && A.back() == 4);

    var B = new vector(int);
    B.push_front(A);
    assert(B.size == 4 && B.front() == 1 && B.back() == 4);

    B.push_front([-3, -2, -1, 0]);
    for (i, b) in zip(B.front()..B.back(), B) do assert(i == b);
  }

  {
    var A = new vector(int);
    for i in 1..10 do A.push_front(i);

    for i in 1..10 by -1 {
      const f = A.front();
      const res = A.pop_front();
      assert(f == res);
      assert(res == i);
      assert(A.size == i - 1);
    }
  }

  writeln("SUCCESS");
}
