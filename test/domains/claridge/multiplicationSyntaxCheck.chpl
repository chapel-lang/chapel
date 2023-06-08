operator *(D1: domain, D2: domain)
{
  param strides = chpl_strideUnion(D1, D2);
  param rank = D1.rank + D2.rank;

  var ranges: rank*range(strides=strides);
  for i in 0..#D1.rank do
    ranges(i) = D1.dim(i);
  for i in 0..#D2.rank do
    ranges(D1.rank+i) = D2.dim(i);

  var D_new: domain(rank,strides=strides) = ranges;
  return D_new;
}


writeln({1..4} * {3..7 by 2});

// var D1 = {1..4};
// var D2 = {3..7 by 2};
// writeln(D1*D2);
