use DSIUtil;

record myDist {
  var _value = new chpl__rectLayoutHelper(new MyDist());
}

class MyDist : BaseDist {}

const d = {1..10, 1..10};

// okay:
var d1 = d dmapped new myDist();

var bd = new myDist(d);
var d2 = d dmapped bd;

var d3: domain(2) dmapped new myDist(d);

// error:
var d4 = d dmapped MyDist(d);
