/*
   This test exercises a feature currently used in distributions.
   We expect to remove this feature in the future once there is a better
   alternative.
 */

class LocDom {
  var dom:domain(1);
}

class LocArr {
  var locDom: LocDom;
  var locArr: [locDom.dom] int;
}

proc run() {
  var myDom   = new LocDom({1..10});
  var myArr   = new LocArr(myDom);
  var newDom   = new LocDom({0..9});
  var locAlias : [newDom.dom] => myArr.locArr;
  var mySlice = new LocArr(locDom=newDom, locArr=>locAlias);

  myArr.locArr[1] = 1;
  myArr.locArr[2] = 2;
  writeln(mySlice.locArr[1]);

  delete mySlice;
  delete newDom;
  delete myArr;
  delete myDom;
}

run();
