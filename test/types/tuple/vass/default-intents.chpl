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

/////////////////////////////////

record RR { var fld: int; }
type Tup864 = (int(8), int);
type Tup6464 = 2*int;

var tupINT = (71, 701);
var tupMix = (81, new RR(901));
var tup864 = (91, 901): Tup864;

proc update() {
  tupINT(0) += 1; tupINT(1) += 1;
  tupMix(0) += 1; tupMix(1).fld += 1;
  tup864(0) += 1; tup864(1) += 1;
}

proc fIn(const in argInt, const in argMix, const in arg6464: Tup6464) {
  writeln("fIn before: ", argInt, "  ", argMix, "  ", arg6464);
  update();
  // we should not observe any updates here
  writeln("fIn after:  ", argInt, "  ", argMix, "  ", arg6464);
}

proc fDflt(argInt, argMix, arg6464: Tup6464) {
  writeln("fDflt before: ", argInt, "  ", argMix, "  ", arg6464);
  update();
  // we should observe only an update to the record component of tupMix
  writeln("fDflt after:  ", argInt, "  ", argMix, "  ", arg6464);
}

fIn(tupINT, tupMix, tup864);
fDflt(tupINT, tupMix, tup864);
