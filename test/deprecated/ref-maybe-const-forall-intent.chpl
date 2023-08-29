// var myArray1: [1..10] int;
// forall i in 1..10 { // should warn
//   myArray1[i] = i;
// }

// var myArray2: [1..10] int;
// forall i in 1..10 with (ref myArray2) { // should not warn
//   myArray2[i] = i;
// }

// var myArray3: [1..10] int;
// forall i in 1..10 { // should not warn
//   write(myArray3[i], " ");
// }
// writeln();

// // should not warn
// var A: [1..10] int;
// forall (i, a) in zip(1..10, A) {
//   a = i;
// }

// // should warn for B only
// // currently warns twice at the same spot. bothersome but not the worst thing
// {
//   var A: [1..10] int = 17;
//   var B: A.type;
//   forall (a,i) in zip(A, A.domain) {
//     ref b = B(i);
//     b = a+1;
//   }
// }

// // should not warn
// var a: [1..10] int;
// var b: a.type;
// [(aa,bb) in zip(a,b)] {
//   aa = bb;
// }

// // should not warn
var c: [1..10] int;
// [cc in c] cc = 1;

// should warn
var myArrayD: [1..10] int;
[i in 1..10] myArrayD(i) = i;
// should not warn
// myArrayD = [i in 1..10] i;
// should not warn
// [(i,dd) in zip(1..10, myArrayD)] dd = i;
// should not warn
forall i in 1..10 with (ref myArrayD) do myArrayD(i) = i;


// {
//   // should not warn
//   var A: [1..10] int;
//   forall a in A {
//     a = 1;
//   }
// }
