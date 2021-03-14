config const n = 4;

var B = [i1 in {1..n}]
          [i2 in {1..i1}]
            i1*100 + i2;

writeln(B);

/*
// cf. this works, however has no shape
var B = [i1 in 1..n]
          [i2 in 1..i1]
            i1*100 + i2;
*/
