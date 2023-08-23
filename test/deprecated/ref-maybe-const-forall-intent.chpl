var x: [1..10] int;
forall i in 1..10 {
  x[i] = i;
}

// should not warn
var A: [1..10] int;
forall (i, a) in zip(1..10, A) {
  a = i;
}

// should warn for B only
{
  var A: [1..10] int = 17;
  var B: A.type;
  forall (a,i) in zip(A, A.domain) {
    ref b = B(i);
    b = a+1;
  }
}

// should not warn
var a: [1..10] int;
var b: a.type;
[(aa,bb) in zip(a,b)] {
  aa = bb;
}

// should not warn
var c: [1..10] int;
[cc in c] cc = 1;

// should warn
var d: [1..10] int;
[i in 1..10] d(i) = i;
// should not warn
d = [i in 1..10] i;
// should not warn
[(i,dd) in zip(1..10, d)] dd = i;
// should not warn
forall i in 1..10 with (ref d) do d(i) = i;


{
  // should not warn
  var A: [1..10] int;
  forall a in A {
    a = 1;
  }
}
