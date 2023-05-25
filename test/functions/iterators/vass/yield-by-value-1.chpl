/*
This test verifies that yielding from an iterator declared
with the default return intent yields everything by copy.

While there, it also verifies that `var tup=(a,b)` copies a and b.
*/

record MyRecord { var field: int; }
proc r(arg:int) do return new MyRecord(arg);

var int1=11, int2=12, int3=13;
var bool1=false, bool2=false, bool3=false;
var rec1=r(71), rec2=r(72), rec3=r(73);
var string1="abc", string2="def", string3="ghi";
var array1=[7,8,9], array2=["x","y","z"], array3=[false,];
var tuple1=(int1,bool1,rec1,string1,array1),
    tuple2=(int2,bool2,rec2,string2,array2),
    tuple3=(int3,bool3,rec3,string3,array3);

proc update() {
  tuple1=(31,false,r(91),"ABC",[4,5,6]);
  tuple2=(32,false,r(92),"DEF",["i","j","k"]);
  tuple3=(33,false,r(93),"GHI",[false]);
  int1=21; int2=22; int3=23;
  bool1=true; bool2=true; bool3=true;
  rec1.field=81; rec2.field=82; rec3.field=83;
  // the point it to mutate the strings in-place to catch refs to them
  string1.buff[0]=65; string2.buff[0]=68; string3.buff[0]=71;
  array1=[9,8,7]; array2=["p","q","r"]; array3=[true,];
}

iter tupleIter() {
  yield (
    (int1, bool1, rec1, string1, array1, tuple1,
      (int2, bool2, rec2, string2, array2, tuple2,
        (int3, bool3, rec3, string3, array3, tuple3))));
}

for idx in tupleIter() {
  writeln("before:  ", idx);
  update();
  writeln("after:   ", idx);
}

  writeln("updated: ",
    (int1, bool1, rec1, string1, array1, tuple1,
      (int2, bool2, rec2, string2, array2, tuple2,
        (int3, bool3, rec3, string3, array3, tuple3))));
