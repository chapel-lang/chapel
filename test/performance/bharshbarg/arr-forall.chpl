//
// Measures both array iteration and array access time
//

config const zipIter = false;
config const n = 1e8:uint;

const space = 1..n;
var data : [space] uint;
var x : uint = 2;

if zipIter {
  forall (i, j) in zip(data, data) with (ref x) do
    x *= i;
} else {
  forall i in data with (ref x) do
    x *= i;
}
