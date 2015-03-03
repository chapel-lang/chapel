config const zipIter = false;
config const n = 1e6:uint;

const space = 1..n;
var x : uint = 2;

if zipIter {
  forall (i, j) in zip(space, space) with (ref x) do
    x *= i;
} else {
  forall i in space with (ref x) do
    x *= i;
}
