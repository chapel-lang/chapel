/* Tests for domain-modifying methods commonly referred to as vector methods */


config const correctness = true;

// push_back
{
  if !correctness then
    writeln('push_back');

  var A: [1..0] int;
  A.push_back(0);
  A.push_back([1,2,3,4]);

  assert(A.equals([0,1,2,3,4]));
}

// push_back self
{
  if !correctness then
    writeln('push_back self');

  var A: [1..4] int = [1,2,3,4];
  A.push_back(A);

  assert(A.equals([1,2,3,4,1,2,3,4]));
}

// push_back slice
{
  if !correctness then
    writeln('push_back slice');

  var A: [1..4] int = [1,2,3,4];
  A.push_back(A[2..3]);

  assert(A.equals([1,2,3,4,2,3]));
}

// push_front
{
  if !correctness then
    writeln('push_front');

  var A: [1..0] int;

  A.push_front(4);
  A.push_front([0,1,2,3]);

  assert(A.equals([0,1,2,3,4]));
}

// push_front self
{
  if !correctness then
    writeln('push_front self');

  var A: [1..4] int = [1,2,3,4];
  A.push_front(A);

  assert(A.equals([1,2,3,4,1,2,3,4]));
}

// push_front slice
{
  if !correctness then
    writeln('push_front slice');

  var A: [1..4] int = [1,2,3,4];
  A.push_front(A[2..3]);

  assert(A.equals([2,3,1,2,3,4]));
}

// insert
{
  if !correctness then
    writeln('insert');

  var A: [1..0] int;

  A.insert(1, 4);
  A.insert(1, [0,3]);
  A.insert(2, [1,2]);

  assert(A.equals([0,1,2,3,4]));
}

// Currently undefined!
// insert self
/*
{
  if !correctness then
    writeln('insert self');

  var A: [1..4] int = [1,2,3,4];
  A.insert(2, A);

  writeln(A);
  assert(A.equals([1,1,2,3,4,2,3,4]));
}
*/

// Currently undefined!
// insert slice
/*
{
  if !correctness then
    writeln('insert slice');

  var A: [1..4] int = [1,2,3,4];
  A.insert(2, A[2..3]);

  assert(A.equals([1,2,3,2,3,4]));
}
*/

//
// Type coercion
//

if !correctness then writeln('Type coercions');

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
