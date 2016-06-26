use StencilDist;
use util;

proc test(dom : domain) {
  param rank = dom.rank;
  var fluff : rank * int;
  for i in 1..rank do fluff(i) = 2;

  const max = dom.expand(fluff*dom.stride);

  var Space = dom dmapped Stencil(dom, fluff=fluff, periodic=true);

  {
    var rs : rank*range;
    rs(1) = 1..dom.dim(1).size/2;
    for param i in 2..rank do rs(i) = Space.dim(i);
    var HalfOne = Space[(...rs)];
    assert(HalfOne._value.fluff == fluff);

    var Data : [Space] int;
    ref Same = Data[Space];
    // Make sure the fluff is still there
    assert(Data._value.dom.fluff == Same._value.dom.fluff);
    ref Half = Data[HalfOne];
    assert(Data._value.dom.fluff == Half._value.dom.fluff);

    forall idx in Space {
      var temp = if isTuple(idx) then idx else (idx,);
      var m = 1;
      for i in temp do m *= i;
      assert(m != 0);
      Data[idx] = m;
    }
    Data.updateFluff();

    // Make sure the slice can see changes
    const sub = Half.localSubdomain();
    const max = sub.expand(fluff*dom.stride);
    for m in max {
      var idx = if isTuple(m) then m else (m,);
      if !Space.member(idx) {
        for param i in 1..rank {
          if idx(i) < Space.dim(i).low then idx(i) += Space.dim(i).size*Space.dim(i).stride;
          else if idx(i) > Space.dim(i).high then idx(i) -= Space.dim(i).size * Space.dim(i).stride;
        }
      }
      assert(Half(m) == Data[idx]);
    }
  }

  {
    var Data : [Space] int;
    ref Actual = Data.noFluffView();
    assert(Actual._value.dom.whole == Actual._value.dom.wholeFluff);

    Data[Space.low] = 42;
    // Ensure that we didn't somehow create a new array with noFluffView
    assert(Actual[Space.low] == Data[Space.low]);

    forall idx in Space {
      var temp = if isTuple(idx) then idx else (idx,);
      var m = 1;
      for i in temp do m *= i;
      assert(m != 0);
      Data[idx] = m;
    }

    // Assert that we can read remote values without having to call updateFluff
    on Locales[numLocales-1] {
      const sub = Data.localSubdomain();
      const max = sub.expand(fluff*dom.stride);
      for idx in max {
        var temp = if isTuple(idx) then idx else (idx,);
        // Only look at indices in 'Space' since no updateFluff call has
        // occurred.
        if Space.member(idx) {
          var m = 1;
          for i in temp do m *= i;
          assert(Actual[idx] == m);
        }
      }
    }
  }
}

test({1..40});
test({1..10, 1..10});
test({10..50, 10..50});
test({1..10, 1..10, 1..10});
writeln("SUCCESS");
