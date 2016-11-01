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

var myDom   = new LocDom({1..10});
var myArr   = new LocArr(myDom);
var locAlias: [myDom.dom] => myArr.locArr;
var mySlice = new LocArr(locDom=myArr.locDom, locArr=>locAlias);

myArr.locArr[1] = 1;
writeln(mySlice.locArr[1]);

delete mySlice;
delete myArr;
delete myDom;

