config param case = 1;

var A: [1..10, 1..10] real;
var X: [1..10] real;

proc zipem(X,Y) {
  forall (x,y) in zip(X, Y) do
    writeln((x,y));
}

select case {
  when 1 do zipem(A, X);
  when 2 do zipem(X, A);
  when 3 do zipem(A.domain, X.domain);
  when 4 do zipem(X.domain, A.domain);
  when 5 do zipem(1..10, A);
  when 6 do zipem(A, 1..10);
}
