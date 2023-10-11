var myArray1: [1..10] int;
forall i in 1..10 { // should warn
  myArray1[i] = i;
}

forall i in 1..10 with (ref myArray1) { // should not warn
  myArray1[i] = i;
}

var myArray2: [1..10] int;
forall i in 1..10 with (ref myArray2) { // should not warn
  myArray2[i] = i;
}

var myArray3: [1..10] int;
forall i in 1..10 { // should not warn
  write(myArray3[i], ",");
}
writeln();

// should not warn
var A: [1..10] int;
forall (i, a) in zip(1..10, A) {
  a = i;
}

// should warn for B only
{ // note that the warning for B is thrown twice. I don't know why, but we might just have to live with it for now
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
var myArrayD: [1..10] int;
[i in 1..10] myArrayD(i) = i;
// should not warn
myArrayD = [i in 1..10] i;
// should not warn
[(i,dd) in zip(1..10, myArrayD)] dd = i;
// should not warn
forall i in 1..10 with (ref myArrayD) do myArrayD(i) = i;


{
  // should not warn
  var A: [1..10] int;
  forall a in A {
    a = 1;
  }
}

{
  // should warn
  var A: [1..10] int;
  forall i in A.domain {
    A[i] = i;
  }
}


{
  proc mod(ref a) do a += 1;
  var myArray1: [1..10] int;
  forall i in 1..9
    do myArray1.localAccess(i) = 1; // should warn

  forall i in 1..9
    do myArray1[i..i+1] = 2; // should warn
  forall i in 1..9
    do myArray1.localSlice(i..i+1) = 3; // should warn

  forall i in 1..9
    do myArray1[{i..i+1}] = 4; // should warn
  forall i in 1..9
    do myArray1.localSlice({i..i+1}) = 4; // should warn

  forall i in 1..9
    do mod(myArray1[i..i+1]); // should warn

  record R {
    var A: [1..10] int;
    var x: int;
    proc ref foo() {
      forall i in 1..10 {
        A[i] = i; // should warn
        // x = 10; // error
      }
      forall i in 1..10 with (ref this) {
        A[i] = i;
        x = 10;
      }
    }
  }
  var r = new R();
  r.foo();
}

{
  proc f(ref x: int) do x = 3;
  const Keys = 1..3;
  var Counts: [Keys] int;
  forall key in Keys { // should warn
    f(Counts[key]);
  }
}

{
  var A: [1..10] int;
  var B: [1..10] int;
  forall i in A.domain with (ref B) { // should not warn for A or B
    B[i] = A[i] * 10 + A.localAccess[i];
  }
}
