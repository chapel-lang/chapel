
use Vector;

proc main() {
  // Inserting into an empty vector
  {
    var A : vector(int);
    A.insert(0, 10);
    assert(A.size == 1 && A.front() == 10);

    var B : vector(int);
    B.insert(0, [1, 2, 3, 4]);
    assert(B.size == 4 && B.front()..B.back() == 1..4);

    var C : vector(int);
    C.insert(0, B);
    assert(C.size == 4 && C.front()..C.back() == 1..4);
  }

  // insert into populated vector
  {
    var A : vector(int);
    for i in 1..10 do A.push_back(i);
    A.insert(A.size/2, 999);
    writeln("int in middle: ", A);

    var B : vector(int);
    for i in 1..10 do B.push_back(i);
    B.insert(B.size/2, [0,0,0,0]);
    writeln("array in middle: ", B);
    
    var C : vector(int);
    for i in 1..10 do C.push_back(i);
    C.insert(C.size/2, new vector([0,0,0,0]));
    writeln("vector in middle: ", C);
  }

  // insert at end of vector
  {
    var A : vector(int);
    for i in 1..4 do A.insert(A.size, i);
    assert(A.equals([1,2,3,4]));
  }

  // remove from ends
  {
    var A : vector(int);
    for i in 1..4 do A.push_back(i);
    A.remove(0);
    assert(A.equals([2,3,4]));
    A.remove(A.size-1);
    assert(A.equals([2,3]));

    while A.isEmpty() == false do
      A.remove(0);
    assert(A.size == 0);
  }

  // remove from middle
  {
    var A : vector(int);
    for i in 1..10 do A.push_back(i);
    const mid = A.size / 2;
    for 1..4 do A.remove(mid - 2); // remove middle few
    assert(A.equals([1,2,3,8,9,10]), A);
  }

  // remove via ranges
  {
    var A : vector(int);
    for i in 1..100 do A.push_back(i);
    A.remove(2.. by 3); // remove every third element

    var B : vector(int);
    for i in 1..100 by 3 {
      B.push_back(i);
      B.push_back(i+1);
    }
    assert(A.equals(B));

    var C : vector(int);
    for i in 1..10 do C.push_back(i);
    C.remove(C.size/2 ..);
    assert(C.equals([1,2,3,4,5]));

    var D : vector(int);
    for i in 1..10 do D.push_back(i);
    D.remove(0..#D.size/2);
    assert(D.equals([6,7,8,9,10]), D);

    var E = new vector([1,2,3,4,5]);
    E.remove(..);
    assert(E.isEmpty());
  }
    

}
