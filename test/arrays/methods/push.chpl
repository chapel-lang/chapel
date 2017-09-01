/* Promoted push operations */

// push_back(array)
{
  var A: [1..0] real;

  var B = [1,2,3,4];
  A.push_back(B);

  assert(A.equals(B));
}

// push_front(array)
{
  var A: [1..0] real;

  var B = [1,2,3,4];
  A.push_front(B);

  B.reverse();
  assert(A.equals(B));
}

// push_back(tuple)
{
  var A: [1..0] real;

  var B = [1,2,3,4];
  A.push_back((1,2,3,4));

  assert(A.equals(B));
}

// push_front(tuple)
{
  var A: [1..0] real;

  var B = [1,2,3,4];
  A.push_front((1,2,3,4));

  B.reverse();
  assert(A.equals(B));
}

// push_back(array) where array is eltType
{
  var A: [1..0] [1..3] int;
  A.push_back([1,2,3]);
  A.push_back([[1,2,3], [1,2,3]]);

  for a in A do assert(a.equals([1,2,3]));
  assert(A.size == 3);
}

// push_back(tuple) where tuple is eltType
{
  var A: [1..0] 3*int;
  A.push_back((1,2,3));
  A.push_back([(1,2,3), (1,2,3)]);

  for a in A do assert(a == (1,2,3));
  assert(A.size == 3);
}
