class MyDist : BaseDist {}

const d = {1..10, 1..10};

// okay:
var d1 = d dmapped new dmap(new MyDist(d));

var bd = new dmap(new MyDist(d));
var d2 = d dmapped bd;

var d3: domain(2) dmapped new dmap(new MyDist(d));

// error:
var d4 = d dmapped MyDist(d);
