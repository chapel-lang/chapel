use Map;

// show that the 'parSafe' argument triggers an unstable warning on all 'map' initializers:

var m1 = new map(string,int,parSafe=true);
var m2 = new map(string,int,parSafe=false);

m1.addOrReplace("a",1);
m2["a"] = 1;
