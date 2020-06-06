// minmax tests on arrays
var a : [1..10] int = [10,21,42,17,3,2,55,101,2,3];
var b : [1..10] int = [10,-21,42,-17,3,2,-55,-101,2,3];
var c : [1..10] int;
writeln(minmax reduce a);
writeln(minmax reduce b);
writeln(minmax reduce c);
writeln(minmax scan a);
writeln(minmax scan b);
writeln(minmax scan c);

// minmax test on list
private use List;
var emptyList = new list(int);
writeln(minmax reduce emptyList);

// minmax test with reduce intent
var tup = (999,0);
forall i in 1..100 with (minmax(int) reduce tup) {
  if i < tup(0) then
    tup(0) = i;
  if i > (tup(1)) then
    tup(1) = i;
}
writeln(tup);

// minmax test with reduce intent using reduce= operator
var mm = (999,-999);
forall elm in a with (minmax(int) reduce mm) {
  mm reduce= elm;
}
writeln(mm);
