class C {
  var dom : domain(1);
  var sps : sparse subdomain(dom);
  var N : [sps] int;
}
var d = {1..10};
var s : sparse subdomain(d) = (1,5);
var c = new owned C(d,s);
writeln("c = ", c);
c.sps += (7,);
c.sps += (9);
writeln("c = ", c);
