use Version;

var x = 2, y = 1, z = 3, q = 0;

const v2_1_0 = createMutableVersion(x,y);
const v2_1_1 = createMutableVersion(x,y,y);
const v2_2_0 = createMutableVersion(x,x,q);
const v2_2_1 = createMutableVersion(x,x,y);
const v3_1_0 = createMutableVersion(z,y);
const v3_1_1 = createMutableVersion(z,y,y);

const v2_1_0_p = createMutableVersion(x,y,q,"aaa");
const v2_1_1_p = createMutableVersion(x,y,y,"bbb");
const v2_2_0_p = createMutableVersion(x,x,commit="ccc");
const v2_2_1_p = createMutableVersion(x,x,y,"ddd");
const v3_1_0_p = createMutableVersion(z,y,q,"eee");
const v3_1_1_p = createMutableVersion(z,y,y,"fff");

var v3_1_1copy = createMutableVersion(2,2,2,"xyz");
v3_1_1copy = v3_1_1;
var v3_1_1_pcopy = createMutableVersion(0,0,1);
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
  writeln("version " + v1:string);
  writeln("version " + v2:string);
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
  writeln("version " + v1:string);
  writeln("version " + v2:string);
  writeln("------------------");
  writeln("== : ", v1 == v2);
  writeln("!= : ", v1 != v2);
  writeln("<  : ", v1 < v2);
  writeln("<= : ", v1 <= v2);
  writeln();
}

proc compareGTVersions(v1, v2) {
  writeln("Comparing versions:");
  writeln("version " + v1:string);
  writeln("version " + v2:string);
  writeln("------------------");
  writeln("== : ", v1 == v2);
  writeln("!= : ", v1 != v2);
  writeln(">  : ", v1 > v2);
  writeln(">= : ", v1 >= v2);
  writeln();
}

