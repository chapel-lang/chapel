// Ensure '?' can be applied to array and domain types, as well as plain ints.

const DOM = {1..1};

class MyDefaultAssociativeArr {
  type eltType;
  var data1 : [DOM] (eltType?);
}

proc test1(type T1) {
  // mimic creating an associative array
  var myArray = new unmanaged MyDefaultAssociativeArr(eltType=T1);
  writeln(myArray.type:string, " : ", myArray);
}


proc test2(type T2) {
  // a simpler reproducer for a former bug
  var data2: [DOM] (T2 ?);
  writeln(data2.type:string, " : ", data2);
}

type ArrType = [DOM] int;

test1(int);
test1(DOM.type);
test1(ArrType);

test2(int);
test2(DOM.type);
test2(ArrType);
