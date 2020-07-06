
use BlockDist;
use StencilDist;
use CyclicDist;

config param zipRange = true;

proc test(Orig : domain) {
  var Copy = Orig;
  test(Orig, Copy);
}

proc makeArray(D : domain, type eltType = int) {
  var ret : [D] eltType;
  var cur = 0;
  for i in D {
    ret[i] = cur;
    cur += 1;
  }
  return ret;
}

proc test(DA : domain, DB : domain) {
  var A = makeArray(DA);
  var B = makeArray(DB);
  test(A,B);
}

proc test(A : [?DA], B : [?DB]) {
  writeln("--- ", A.type:string, " vs. ", B.type:string, " ---");
  DA.dist.displayRepresentation();
  writeln();
  DB.dist.displayRepresentation();
  writeln();

  if !zipRange {
    forall (a,b) in zip(A,B) do
      a += b;
  }
  else {
    forall (a,b,i) in zip(A,B, 0..) do
      a += b+i;
  }
  writeln("----------");
  writeln();

  var cur = 0;
  for i in DA {
    if !zipRange {
      assert(A[i] == cur * 2);
    }
    else {
      assert(A[i] == cur * 3);
    }
    cur += 1;
  }
}

enum Dist {
  block,
  stencil,
  cyclic
}
use Dist;

proc make(param dist : Dist, dom : domain, box = dom, targetLocales = Locales, startIdx = (dom.first,)) {
  if dist == cyclic then
    return dom dmapped Cyclic(startIdx,targetLocales=targetLocales);
  else if dist == block then
    return dom dmapped Block(box, targetLocales=targetLocales);
  else if dist == stencil then
    return dom dmapped Stencil(box, targetLocales=targetLocales);
}

proc main() {
  const one = {1..10};
  const zero = {0..9};
  const first = [Locales[0], Locales[1]];
  const second = [Locales[2], Locales[3]];

  // check that simple copies will trigger fast followers
  test(make(block,one));
  test(make(stencil, one));
  test(make(cyclic, one));

  // check variants that shouldn't match

  // - different targetLocales
  test(make(block,one,targetLocales=first), make(block,one,targetLocales=second));
  test(make(stencil,one,targetLocales=first), make(stencil,one,targetLocales=second));
  test(make(cyclic,one,targetLocales=first), make(cyclic,one,targetLocales=second));

  // - different indices
  test(make(block,one),make(block,zero));
  test(make(stencil,one),make(stencil,zero));
  test(make(cyclic,one),make(cyclic,zero));

  // - different bounding box
  test(make(block,one,{0..99}),make(block,one,{1..100}));
  test(make(stencil,one,{0..99}),make(stencil,one,{1..100}));

  // - different startIdx
  test(make(cyclic,one),make(cyclic,one,startIdx=(2,)));

  // - different eltTypes
  test(makeArray(make(block,one), real), makeArray(make(block,one), int));
  test(makeArray(make(cyclic,one), real), makeArray(make(cyclic,one), int));
  test(makeArray(make(stencil,one), real), makeArray(make(stencil,one), int));
}
