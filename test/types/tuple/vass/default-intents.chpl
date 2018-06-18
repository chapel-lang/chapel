var tupRange = (1..2,);
test(tupRange, tupRange, (3..4,));     // range: passed by const in

var tupInt = (3,);
test(tupInt, tupInt, (7,));            // int: by const in

var tupString = ("hi",);
test(tupString, tupString, ("bye",));  // string: by const ref

var tupDom = ({1..2},);
test(tupDom, tupDom, ({3..4},));       // domain: by const ref

const Arr = [1,2];
var tupArr = (Arr,);
var tupArr2 = ([3,4],);
test(tupArr, tupArr, tupArr2);         // array: by maybe-const ref

proc test(tuple, ref outer, newValue) {
  writeln(outer);
  outer = newValue;
  writeln(tuple);
}
