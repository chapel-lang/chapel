
type DomType1 = domain(1);
const init1dom = {7770..7771};

type DomType2 = domain(2);
const init2dom = {110001..110002, 330004..330005};

type ArrType1 = [init1dom] int;
const init1arr: ArrType1 = 11011;

type ArrType2 = [init2dom] int;
const init2arr: ArrType2 = 33033;

writeln("starting");
test();
writeln("done");

proc test() {

  var dom1: DomType1;
  var dom2: DomType2;
  var arr1: ArrType1;
  var arr2: ArrType2;
  var s$: sync int;

  begin with (ref dom1, ref dom2) {
    writeln("in begin");
    writeln("dom1", " ", dom1, ".");
    writeln("dom2", " ", dom2, ".");
    writeln("arr1", " ", arr1, ".");
    writeln("arr2", " ", arr2, ".");

    dom1 = init1dom;
    dom2 = init2dom;
    arr1 = init1arr;
    arr2 = init2arr;
    s$.writeEF(1);
  }

  s$.readFE();
  writeln("after begin");
  writeln("dom1", " ", dom1, ".");
  writeln("dom2", " ", dom2, ".");
  writeln("arr1", " ", arr1, ".");
  writeln("arr2", " ", arr2, ".");
}
