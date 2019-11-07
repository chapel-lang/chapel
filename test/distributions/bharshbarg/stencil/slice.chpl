use StencilDist;
use util;

config const debug = false;

proc test(dom : domain) {

  if debug then writeln("Testing domain ", dom);

  param rank = dom.rank;
  var fluff : rank * int;
  for i in 0..#rank do fluff(i) = 2;

  const max = dom.expand(fluff*dom.stride);

  var Space = dom dmapped Stencil(dom, fluff=fluff, periodic=true);
  var abstr : rank*int;
  for i in 0..#rank do abstr(i) = abs(dom.dim(i).stride);

  {
    // Create a tuple of ranges where the first dimension is halved
    var rs : rank*Space.dim(0).type;
    rs(0) = dom.dim(0)[dom.dim(0).low..dom.dim(0).high/2];
    for param i in 1..rank-1 do rs(i) = dom.dim(i);

    var HalfDom = Space[(...rs)];

    // Slicing used to drop the fluff, make sure that doesn't happen
    assert(HalfDom._value.fluff == fluff);

    var Data : [Space] int;
    ref Same = Data[Space];

    assert(Data._value.dom.fluff == Same._value.dom.fluff);
    ref Half = Data[HalfDom];
    assert(Data._value.dom.fluff == Half._value.dom.fluff);

    // Fill our array with dummy values
    forall idx in Space {
      var temp = if isTuple(idx) then idx else (idx,);
      var m = 1;
      for i in temp do m *= i;
      assert(m != 0);
      Data[idx] = m;
    }
    Data.updateFluff();

    // Make sure the slice can see changes. Relies on the current locale being
    // on a corner or edge of Space so that we check a periodic region.
    const sub = Half.localSubdomain();
    const max = sub.expand(fluff*abstr);
    for m in max {
      var idx = if isTuple(m) then m else (m,);

      // Translate our index if needed
      if !Space.contains(idx) {
        for param i in 0..rank-1 {
          if idx(i) < Space.dim(i).low then idx(i) += Space.dim(i).size * abstr(i);
          else if idx(i) > Space.dim(i).high then idx(i) -= Space.dim(i).size * abstr(i);
        }
      }

      assert(Half(m) == Data[idx]);
    }
  }

  {
    // Test the 'noFluffView' feature of StencilDist, which gives us an
    // array that behaves like a BlockDist
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
      const max = sub.expand(fluff*abstr);
      for idx in max {
        var temp = if isTuple(idx) then idx else (idx,);
        // Look at indices that should be in the cache, but are not in the
        // periodic region.
        if !sub.contains(idx) && dom.contains(idx) {
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
test({1..15, 1..15} by 2);
test({-10..60, 5..75, -100..-23} by 4);
// TODO: domain with negative stride?

writeln("SUCCESS");
