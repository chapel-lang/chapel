use util;

config const debug = false;
config param maxDims = 4;

proc helper(param dim : int, param rank : int) {
  if rank < 1 then compilerError("rank must be greater than 1!");

  if dim == 1 {
    if rank == 1 then return (1,);
    else return (1, (...helper(dim-1, rank-1)));
  } else {
    if rank == 1 then return (0..1 by 1,);
    else return (0..1 by 1, (...helper(dim-1, rank-1)));
  }
}

iter ranges() {
  yield 1..10 by 1;
  yield 1..5 by 1;
}

iter variations(in template, param rank = template.size) {
  if isRange(template(rank-1)) {
    for r in ranges() {
      template(rank-1) = r;
      if rank == 1 then yield template;
      else {
        for v in variations(template, rank-1) do yield v;
      }
    }
  }
  else {
    if rank == 1 then yield template;
    else {
      for v in variations(template, rank-1) do yield v;
    }
  }
}


for param rank in 2..maxDims {
  var dom : domain(rank, stridable=true);
  {
    var r : rank*range(stridable=true);
    for i in 0..#rank do r(i) = 1..10;
    var temp : domain(rank, stridable=true) = r;
    dom = temp;
  }

  var A, B : [dom] int;

  var smaller : domain(rank-1, stridable=true);
  {
    var r : (rank-1)*range(stridable=true);
    for i in 0..#(rank-1) do r(i) = 1..10;
    var temp : domain(rank-1, stridable = true) = r;
    smaller = temp;
  }
  
  var S : [smaller] int;

  for param dim in 1..rank {
    var dest = helper(dim, rank);

    for left in variations(dest) {
      for param other in 1..rank {
        var src = helper(other, rank);
        for right in variations(src) {
          stridedAssign(A, left, B, right, debug);
        }
      }

      stridedAssign(S, S.domain, B, left);
    }
  }
}

// Simple test of multiple rank changes in the same slice
{
  var A : [1..5, 1..5, 1..5, 1..5] int;
  var B : [1..5, 1..5] int;

  stridedAssign(B, B.domain, A, (1..5, 3, 1..2, 1));
}

