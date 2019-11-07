proc *(D1: domain, D2: domain)
{
  param stridable = D1.stridable || D2.stridable;
  param rank = D1.rank + D2.rank;

  var ranges: rank*range(stridable=stridable);
  for i in 0..#D1.rank do
    ranges(i) = D1.dim(i);
  for i in 0..#D2.rank do
    ranges(D1.rank+i) = D2.dim(i);

  var D_new: domain(rank,stridable=stridable) = ranges;
  return D_new;
}


writeln({1..4} * {3..7 by 2});

// var D1 = {1..4};
// var D2 = {3..7 by 2};
// writeln(D1*D2);
