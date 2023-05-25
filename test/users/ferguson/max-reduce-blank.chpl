proc main() {
  var A:[1..10] (int,int) = [i in 1..10] (i, 2*i);
  var maxI = max reduce [(i,_) in A] i;
  var maxJ = max reduce [(_,j) in A] j;

  writeln(maxI, " ", maxJ);
}
