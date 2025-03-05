config const option = 1;
var A = [1.2, 3.4, 5.6, 7.8];
var B: [0..1, 0..2] real = ((1.1, 1.2, 1.3),
                            (2.1, 2.2, 2.3));
var C = [[1.0, 2.0], [3.0, 4.0]];

select option {
  when 1 do
    cast(A, [1..10] int);
  when 2 do
    cast(A, [1..3] int);
  when 3 do
    cast(B, [1..3, 1..2] real(32));
  when 4 do
    cast(B, [1..2, 1..1] real(32));
  when 5 do
    cast(C, [1..1] [1..6] real(32));
  when 6 do
    cast(C, [1..2] [1..6] real(32));
}

proc cast(arr, type t) {
  var X = arr: t;
  writeln(X);
}