/* Tests for domain-modifying methods commonly referred to as vector methods */

// push_back
{
  var A: [1..0] int;
  A.push_back(0);
  A.push_back([1,2,3,4]);

  assert(A.equals([0,1,2,3,4]));
}

// push_front
{
  var A: [1..0] int;

  A.push_front(4);
  A.push_front([0,1,2,3]);

  assert(A.equals([0,1,2,3,4]));
}

// insert
{
  var A: [1..0] int;

  A.insert(1, 4);
  A.insert(1, [0,3]);
  A.insert(2, [1,2]);

  assert(A.equals([0,1,2,3,4]));
}

//
// Type coercion
//

// push_back
{
  var A: [1..0] real;
  A.push_back(0);
  A.push_back([1,2,3,4]);

  assert(A.equals([0,1,2,3,4]));
}

// push_front
{
  var A: [1..0] real;

  A.push_front(4);
  A.push_front([0,1,2,3]);

  assert(A.equals([0,1,2,3,4]));
}

// insert
{
  var A: [1..0] real;

  A.insert(1, 4);
  A.insert(1, [0,3]);
  A.insert(2, [1,2]);

  assert(A.equals([0,1,2,3,4]));
}

//
// Arrays of arrays
//

// push_back
{
  var A: [1..0] [1..2] int;
  const B: [1..3] [1..2] int = [[1,1],[2,2],[3,3]];

  A.push_back(B);

  assert(A.equals(B));
}

// push_front
{
  var A: [1..0] [1..2] int;
  const B: [1..3] [1..2] int = [[1,1],[2,2],[3,3]];

  A.push_front(B);

  assert(A.equals(B));
}

// insert
{
  var A: [1..0] [1..2] int;
  const B: [1..3] [1..2] int = [[1,1],[2,2],[3,3]];

  A.insert(1, B);

  assert(A.equals(B));
}
