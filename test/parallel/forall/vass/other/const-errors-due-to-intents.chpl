// Test of error reporting for const errors due to forall intents.

var i1, i2, i3, i4: int;

var A = [1,2,3];

proc useit(ref arg: int) {}

forall a in A with (const i2, const in i3, const ref i4) {
  i1 = 1;
  i2 = 2;
  i3 = 3;
  i4 = 4;
  useit(i1);
  useit(i2);
  useit(i3);
  useit(i4);
}

forall a1 in A do
  forall a2 in A do
    forall a3 in A with (ref i1) do  // illegal: 'i1' is const in 'forall a2'
      i1 = 8;

forall a1 in A do
  forall a2 in A do
    i2 = 8;  // illegal: 'i2' is const in 'forall a2'
