private use List;
use util;

config const n = 15;

config const printCount = false;

config const debug = false;

iter helper(param dim : int, param rank : int, ranges) {
  if dim == rank {
    for i in ranges[dim] {
      if i.size > 0 then yield (i,);
    }
  } else {
    for i in ranges[dim] {
      if i.size > 0 {
        for j in helper(dim+1, rank, ranges) {
          yield (i, (...j));
        }
      }
    }
  }
}

proc buildSlices(param rank : int, Orig : domain(rank, stridable=true)) {
  //
  // The commented out code below triggers a strange bug related to the RTTI
  // info for domains not being initialized correctly. However, using a "new"
  // expression instead provides a workaround to this problem.
  //
  // See: #13808
  //
  // var ret : list(domain(rank, stridable=true));
  //
  var ret = new list(domain(rank, stridable=true));

  var innerDom = {1..0};
  var perDim : [1..rank] [innerDom] range(stridable=true);

  for i in 0..#rank {
    const cur = Orig.dim(i);
    const low = cur.low;
    const high = cur.high;
    const size = cur.size;
    const mid = low + size/2;
    const quart = size/4;
    const str = Orig.dim(i).stride;

    var mine : list(range(stridable=true));

    if cur.size == 1 {
      mine.append(cur by str);
    } else {
      // dense ranges
      mine.append(cur by str);
      mine.append(low..#size/2 by str);

      // Try to have a case where we test something like this:
      // {1..10, 1..5} = {1..10, 1..10 by 2}
      mine.append(low..high by 2*str);

      // single-element slice
      mine.append(low..low by str);

      if rank <= 2 {
        // single-element due to really large stride
        mine.append(low..high by (size+1)*str);
      }
      if rank <= 3 {
        // strides that either do or do not fit in the ranges
        var even, odd : int = 2 * str;
        while !(size % even == 0) do even += str;
        while (size % odd == 0) do odd += str;

        mine.append(low..high by even);
        mine.append(low..high by odd);
        mine.append((low+quart)..high by even);
        mine.append((low+quart)..high by odd);
        mine.append(low..mid by even);
      }
    }

    if innerDom.size < mine.size then innerDom = {1..mine.size};
    perDim[i+1][1..mine.size] = mine;
  }

  for r in helper(1, rank, perDim) {
    var dom : domain(rank, stridable=true) = r;
    ret.append(dom);
  }
  
  return ret;
}


proc test(param rank : int, Dom) {

  var A : [Dom] int;
  var B : [Dom] int;

  var slices = buildSlices(rank, Dom);

  A = 1;

  // If the value '3' appears anywhere in 'A', then we've read incorrectly
  B = 3;

  var count = 0;
  for i in 0..slices.size-1 {
    for j in i..slices.size-1 {
      var sa = slices[i];
      var sb = slices[j];

      const ret = stridedAssign(A, sa, B, sb, debug);
      if ret == 1 then count += 1;
    }
  }

  if printCount then writeln("Performed ", count, " assignments");
}

proc testReindex(param rank : int, Dom) {

  var A : [Dom] int;
  var B : [Dom] int;

  var slices = buildSlices(rank, Dom);

  A = 1;

  // If the value '3' appears anywhere in 'A', then we've read incorrectly
  B = 3;

  ref refA = A.reindex(Dom);
  ref refB = B.reindex(Dom);

  var count = 0;
  for i in 0..slices.size-1 {
    for j in i..slices.size-1 {
      var sa = slices[i];
      var sb = slices[j];

      const ret = stridedAssign(refA, sa, refB, sb, debug);
      if ret == 1 then count += 1;
    }
  }

  if printCount then writeln("Performed ", count, " assignments");
}

proc makeDom(param rank : int, low : int, str = 1) {
  var r : rank*range(stridable=true);
  for param i in 0..rank-1 do r(i) = low.. by str # n;
  return r;
}

proc main() {
  for param rank in 1..3 {
    var negative : domain(rank, stridable=true) = makeDom(rank, -(n+5));
    var positive : domain(rank, stridable=true) = makeDom(rank, 1);
    var str : domain(rank, stridable=true) = makeDom(rank, 1, 3);

    test(rank, negative);
    test(rank, positive);
    test(rank, str);
  }
  test(3, {1..10, 1..1, 1..10} by 1);

  // 1-length dimension due to a stride larger than the dimension's size
  test(3, {1..10, 1..3 by 4, 1..10});

  var four : domain(4, stridable=true) = makeDom(4, 1);
  test(4, four);

  testReindex(1, {1..10} by 1);
  testReindex(2, {1..5, 1..5} by 1);
}
