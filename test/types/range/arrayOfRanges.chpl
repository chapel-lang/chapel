// Motivated by getting the subranges from the range leader iterator
var arr: [0..4] range = [1..2, 3..4, 6..7, 5..5, -1..0];

writeln(arr);

var arr2: [arr.domain] int;

// I wanted to store the size of each of the ranges into a separate array, but
// writing it like this didn't work.
forall (i, r) in zip(arr.domain, arr) {
  arr2[i] = r.size;
}
/* Had to use this instead:
for i in arr.domain {
  arr2[i] = arr[i].size;
}
*/

writeln(arr2);
