var m : integer = 100; 
var n : integer = 200;

var Grid : array (1..4, 1..4) of locale
         = reshape(Locales(1..16), (1..4, 1..4));

var Mat : domain(2) distribute(block2) to (Grid)
        = (1..m, 1..n);

var A, B, C : array Mat of float;

A = [i,j:Mat] (if i=j then 1 else 0);
B = [i,j:Mat] (if i=(n-j+1) then 1 else 0);

C = A + B;

write(C);

