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
  var ret : [1..0] domain(rank, stridable=true);

  var innerDom = {1..0};
  var perDim : [1..rank] [innerDom] range(stridable=true);

  for i in 1..rank {
    const cur = Orig.dim(i);
    const low = cur.low;
    const high = cur.high;
    const size = cur.size;
    const mid = low + size/2;
    const quart = size/4;
    const str = Orig.dim(i).stride;

    var mine : [1..0] range(stridable=true);

    if cur.length == 1 {
      mine.push_back(cur by str);
    } else {
      // dense ranges
      mine.push_back(cur by str);
      mine.push_back(low..#size/2 by str);

      // Try to have a case where we test something like this:
      // {1..10, 1..5} = {1..10, 1..10 by 2}
      mine.push_back(low..high by 2*str);

      // single-element slice
      mine.push_back(low..low by str);

      if rank <= 2 {
        // single-element due to really large stride
        mine.push_back(low..high by (size+1)*str);
      }
      if rank <= 3 {
        // strides that either do or do not fit in the ranges
        var even, odd : int = 2 * str;
        while !(size % even == 0) do even += str;
        while (size % odd == 0) do odd += str;

        mine.push_back(low..high by even);
        mine.push_back(low..high by odd);
        mine.push_back((low+quart)..high by even);
        mine.push_back((low+quart)..high by odd);
        mine.push_back(low..mid by even);
      }
    }

    if innerDom.size < mine.size then innerDom = {1..mine.size};
    perDim[i][1..mine.size] = mine;
  }

  for r in helper(1, rank, perDim) {
    var dom : domain(rank, stridable=true) = r;
    ret.push_back(dom);
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
  for i in 1..slices.size {
    for j in i..slices.size {
      var sa = slices[i];
      var sb = slices[j];

      const ret = stridedAssign(A, sa, B, sb, debug);
      if ret == 1 then count += 1;
    }
  }

  if printCount then writeln("Performed ", count, " assignments");
}

proc makeDom(param rank : int, low : int, str = 1) {
  var r : rank*range(stridable=true);
  for param i in 1..rank do r(i) = low.. by str # n;
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
}
