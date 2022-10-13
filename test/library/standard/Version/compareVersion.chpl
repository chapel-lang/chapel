use Version;

var x = 2, y = 1, z = 3, q = 0;

var v2_1_0 = new version(x,y);
var v2_1_1 = new version(x,y,y);
var v2_2_0 = new version(x,x,q);
var v2_2_1 = new version(x,x,y);
var v3_1_0 = new version(z,y);
var v3_1_1 = new version(z,y,y);

var v2_1_0_p = new version(x,y,q,"aaa");
var v2_1_1_p = new version(x,y,y,"bbb");
var v2_2_0_p = new version(x,x,commit="ccc");
var v2_2_1_p = new version(x,x,y,"ddd");
var v3_1_0_p = new version(z,y,q,"eee");
var v3_1_1_p = new version(z,y,y,"fff");

var v3_1_1copy = new version(2,2,2,"xyz");
v3_1_1copy = v3_1_1;
var v3_1_1_pcopy = new version(0,0,1);
v3_1_1_pcopy = v3_1_1_p;
compareVersions(v3_1_1copy, v3_1_1);
compareVersions(v3_1_1_pcopy, v3_1_1_p);

compareVersions(v2_1_0, v2_1_0);
compareVersions(v2_1_0_p, v2_1_0_p);
compareLTVersions(v2_1_0, v2_1_0_p);
compareGTVersions(v2_1_0_p, v2_1_0);

compareBothVersions(v2_1_0, v2_1_1);
compareBothVersions(v2_1_0, v2_2_0);
compareBothVersions(v2_1_0, v2_2_1);
compareBothVersions(v2_1_0, v3_1_0);
compareBothVersions(v2_1_0, v3_1_1);

compareBothVersions(v2_1_0, v2_1_1_p);
compareBothVersions(v2_1_0, v2_2_0_p);
compareBothVersions(v2_1_0, v2_2_1_p);
compareBothVersions(v2_1_0, v3_1_0_p);
compareBothVersions(v2_1_0, v3_1_1_p);

compareBothVersions(v2_1_1, v2_2_0);
compareBothVersions(v2_1_1, v2_2_1);
compareBothVersions(v2_1_1, v3_1_0);
compareBothVersions(v2_1_1, v3_1_1);

compareBothVersions(v2_1_1, v2_2_0_p);
compareBothVersions(v2_1_1, v2_2_1_p);
compareBothVersions(v2_1_1, v3_1_0_p);
compareBothVersions(v2_1_1, v3_1_1_p);

compareBothVersions(v2_2_0, v2_2_1);
compareBothVersions(v2_2_0, v3_1_0);
compareBothVersions(v2_2_0, v3_1_1);

compareBothVersions(v2_2_0, v2_2_1_p);
compareBothVersions(v2_2_0, v3_1_0_p);
compareBothVersions(v2_2_0, v3_1_1_p);

compareBothVersions(v2_2_1, v3_1_0);
compareBothVersions(v2_2_1, v3_1_1);

compareBothVersions(v2_2_1, v3_1_0_p);
compareBothVersions(v2_2_1, v3_1_1_p);

compareBothVersions(v3_1_0, v3_1_1);

compareBothVersions(v3_1_0, v3_1_1_p);


proc compareBothVersions(v1, v2) {
  compareVersions(v1,v2);
  compareVersions(v2,v1);
}

proc compareVersions(v1, v2) {
  writeln("Comparing versions:");
  writeln(v1:string);
  writeln(v2:string);
  writeln("------------------");
  writeln("== : ", v1 == v2);
  writeln("!= : ", v1 != v2);
  writeln("<  : ", v1 < v2);
  writeln("<= : ", v1 <= v2);
  writeln(">  : ", v1 > v2);
  writeln(">= : ", v1 >= v2);
  writeln();
}

proc compareLTVersions(v1, v2) {
  writeln("Comparing versions:");
  writeln(v1:string);
  writeln(v2:string);
  writeln("------------------");
  writeln("== : ", v1 == v2);
  writeln("!= : ", v1 != v2);
  writeln("<  : ", v1 < v2);
  writeln("<= : ", v1 <= v2);
  writeln();
}

proc compareGTVersions(v1, v2) {
  writeln("Comparing versions:");
  writeln(v1:string);
  writeln(v2:string);
  writeln("------------------");
  writeln("== : ", v1 == v2);
  writeln("!= : ", v1 != v2);
  writeln(">  : ", v1 > v2);
  writeln(">= : ", v1 >= v2);
  writeln();
}

