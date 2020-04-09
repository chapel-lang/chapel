//This tests directly call chpl_comm_get_strd to 
// 1.- get some elements from B on locale 0 to A on locale 0
// 2.- put some elements from A on locale 0 to B on locale 0


proc TestGetsPuts(A:[], B:[])
{
  A._value.TestGetsPuts(B);
}

proc BlockArr.TestGetsPuts(B)
{
  use SysCTypes;

  param stridelevels=1;
  //Errors below if stridelevels=0 !!
  var dststrides:[1..#stridelevels] size_t;
  var srcstrides: [1..#stridelevels] size_t;
  var count: [1..#(stridelevels+1)] size_t;
  var lid=0; //local locale id

  on Locales[0] {
      dststrides[1]=4;
      srcstrides[1]=2;
      count[1]=2;
      count[2]=4;
      writeln();

      var dest = locArr[0].myElems._value.theData; // can this be myLocArr?
      var srcl = B._value.locArr[lid].myElems._value.theData;
      var dststr=dststrides._value.theData;
      var srcstr=srcstrides._value.theData;
      var cnt=count._value.theData;

// 1.- get some elements from B on locale 0 to A on locale 0
      __primitive("chpl_comm_get_strd",
                 __primitive("array_get",dest,
                             locArr[0].myElems._value.getDataIndex(24)),
                 __primitive("array_get",dststr,dststrides._value.getDataIndex(1)),
                 lid,
                 __primitive("array_get",srcl,
                             B._value.locArr[lid].myElems._value.getDataIndex(8)),
                 __primitive("array_get",srcstr,srcstrides._value.getDataIndex(1)),
                 __primitive("array_get",cnt, count._value.getDataIndex(1)),
                 stridelevels);

      var src = locArr[0].myElems._value.theData; // can this be myLocArr?
      var destl = B._value.locArr[lid].myElems._value.theData;

// 2.- put some elements from A on locale 0 to B on locale 0
      __primitive("chpl_comm_put_strd",
                 __primitive("array_get",destl,
                             B._value.locArr[lid].myElems._value.getDataIndex(16)),
                 __primitive("array_get",dststr,dststrides._value.getDataIndex(1)),
                 lid,
                 __primitive("array_get",src,
                             locArr[0].myElems._value.getDataIndex(2)),
                 __primitive("array_get",srcstr,srcstrides._value.getDataIndex(1)),
                 __primitive("array_get",cnt, count._value.getDataIndex(1)),
                 stridelevels);

  }
}

use BlockDist;

const n: int=50*numLocales;
var Dist = new dmap(new Block({1..n}));

var Dom: domain(1,int) dmapped Dist = {1..n};

var A:[Dom] int(64); //real
var B:[Dom] int(64); //real

proc main(){

  /* write("A Distribution: "); */

  var a,b:int;
  var i:int;
  for (a,i) in zip(A,{1..n}) do a=i;
  for (b,i) in zip(B,{1..n}) do b=i*100;
  writeln("Original vector:");
  writeln("===================");
  writeln("A= ", A);
  writeln("B= ", B);

  TestGetsPuts(A,B);

  writeln("After gets and puts");

  writeln("A= ", A);
  writeln("B= ", B);

  writeln();

}
