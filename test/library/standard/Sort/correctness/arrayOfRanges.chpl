// Motivated by wanting to sort the ranges returned by the range leader iterator
use Sort;

var arr: [0..4] range = [1..2, 3..4, 6..7, 5..5, -1..0];

writeln(arr);

sort(arr);

writeln(arr);
