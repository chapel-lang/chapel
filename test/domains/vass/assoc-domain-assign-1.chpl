test1();
test2();

proc test1() {
type a = int;
var d1: domain(a) = (1,2,3,4,5);
//nondet: writeln(d1);
d1 = (3,4,5);
writeln(d1.contains(1));
var d2: subdomain(d1) = (1,2);
writeln(d2.contains(1));
}

proc test2() {
enum a {b,c,d};
var d1: domain(a);
d1 = (a.b,a.d);
writeln(d1);
var d2: subdomain(d1);
d2 = (a.c,a.d);
writeln(d2);
}
