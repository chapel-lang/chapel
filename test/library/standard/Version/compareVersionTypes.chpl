use Version;

// check that we can compare different version types no matter
// if they are the LHS or RHS of the operator.

const v2_1_0 = new versionValue(2,1);
const v2_1_1 = new versionValue(2,1,1);
const v2_2_0 = new versionValue(2,2,0);
const v2_2_1 = new versionValue(2,2,1);
const v3_1_0 = new versionValue(3,1);
const v3_1_1 = new versionValue(3,1,1);

const v2_1_0_p = new versionValue(2,1,0,"aaa");
const v2_1_1_p = new versionValue(2,1,1,"bbb");
const v2_2_0_p = new versionValue(2,2,commit="ccc");
const v2_2_1_p = new versionValue(2,2,1,"ddd");
const v3_1_0_p = new versionValue(3,1,0,"eee");
const v3_1_1_p = new versionValue(3,1,1,"fff");

var x = 2, y = 1, z = 3, q = 0;

const v2_1_0_c = new version(x,y);
const v2_1_1_c = new version(2,1,1);
const v2_2_0_c = new version(x,x,q);
const v2_2_1_c = new version(x,x,y);
const v3_1_0_c = new version(z,y);
const v3_1_1_c = new version(3,1,1);

const v2_1_0_p_c = new version(x,y,q,"aaa");
const v2_1_1_p_c = new version(x,y,y,"bbb");
const v2_2_0_p_c = new version(x,x,commit="ccc");
const v2_2_1_p_c = new version(x,x,y,"ddd");
const v3_1_0_p_c = new version(z,y,q,"eee");
const v3_1_1_p_c = new version(z,y,y,"fff");


compareVersions(v3_1_1_c, v3_1_1);
compareVersions(v3_1_1, v3_1_1_c);
compareVersions(v3_1_1_p_c, v3_1_1_p);
compareVersions(v3_1_1_p, v3_1_1_p_c);

compareVersions(v2_1_0_c, v2_1_0);
compareVersions(v2_1_0, v2_1_0_c);
compareVersions(v2_1_0_p_c, v2_1_0_p);
compareVersions(v2_1_0_p, v2_1_0_p_c);
compareLTVersions(v2_1_0_c, v2_1_0_p);
compareLTVersions(v2_1_0, v2_1_0_p_c);
compareGTVersions(v2_1_0_p_c, v2_1_0);
compareGTVersions(v2_1_0_p, v2_1_0_c);

compareBothVersions(v2_1_0_c, v2_1_1);
compareBothVersions(v2_1_0, v2_1_1_c);
compareBothVersions(v2_1_0_c, v2_2_0);
compareBothVersions(v2_1_0, v2_2_0_c);
compareBothVersions(v2_1_0_c, v2_2_1);
compareBothVersions(v2_1_0, v2_2_1_c);
compareBothVersions(v2_1_0_c, v3_1_0);
compareBothVersions(v2_1_0, v3_1_0_c);
compareBothVersions(v2_1_0_c, v3_1_1);
compareBothVersions(v2_1_0, v3_1_1_c);

compareBothVersions(v2_1_0_c, v2_1_1_p);
compareBothVersions(v2_1_0, v2_1_1_p_c);
compareBothVersions(v2_1_0_c, v2_2_0_p);
compareBothVersions(v2_1_0, v2_2_0_p_c);
compareBothVersions(v2_1_0_c, v2_2_1_p);
compareBothVersions(v2_1_0, v2_2_1_p_c);
compareBothVersions(v2_1_0_c, v3_1_0_p);
compareBothVersions(v2_1_0, v3_1_0_p_c);
compareBothVersions(v2_1_0_c, v3_1_1_p);
compareBothVersions(v2_1_0, v3_1_1_p_c);

compareBothVersions(v2_1_1_c, v2_2_0);
compareBothVersions(v2_1_1, v2_2_0_c);
compareBothVersions(v2_1_1_c, v2_2_1);
compareBothVersions(v2_1_1, v2_2_1_c);
compareBothVersions(v2_1_1_c, v3_1_0);
compareBothVersions(v2_1_1, v3_1_0_c);
compareBothVersions(v2_1_1_c, v3_1_1);
compareBothVersions(v2_1_1, v3_1_1_c);

compareBothVersions(v2_1_1_c, v2_2_0_p);
compareBothVersions(v2_1_1, v2_2_0_p_c);
compareBothVersions(v2_1_1_c, v2_2_1_p);
compareBothVersions(v2_1_1, v2_2_1_p_c);
compareBothVersions(v2_1_1_c, v3_1_0_p);
compareBothVersions(v2_1_1, v3_1_0_p_c);
compareBothVersions(v2_1_1_c, v3_1_1_p);
compareBothVersions(v2_1_1, v3_1_1_p_c);

compareBothVersions(v2_2_0_c, v2_2_1);
compareBothVersions(v2_2_0, v2_2_1_c);
compareBothVersions(v2_2_0_c, v3_1_0);
compareBothVersions(v2_2_0, v3_1_0_c);
compareBothVersions(v2_2_0_c, v3_1_1);
compareBothVersions(v2_2_0, v3_1_1_c);

compareBothVersions(v2_2_0_c, v2_2_1_p);
compareBothVersions(v2_2_0, v2_2_1_p_c);
compareBothVersions(v2_2_0_c, v3_1_0_p);
compareBothVersions(v2_2_0, v3_1_0_p_c);
compareBothVersions(v2_2_0_c, v3_1_1_p);
compareBothVersions(v2_2_0, v3_1_1_p_c);

compareBothVersions(v2_2_1_c, v3_1_0);
compareBothVersions(v2_2_1, v3_1_0_c);
compareBothVersions(v2_2_1_c, v3_1_1);
compareBothVersions(v2_2_1, v3_1_1_c);

compareBothVersions(v2_2_1_c, v3_1_0_p);
compareBothVersions(v2_2_1, v3_1_0_p_c);
compareBothVersions(v2_2_1_c, v3_1_1_p);
compareBothVersions(v2_2_1, v3_1_1_p_c);

compareBothVersions(v3_1_0_c, v3_1_1);
compareBothVersions(v3_1_0, v3_1_1_c);

compareBothVersions(v3_1_0_c, v3_1_1_p);
compareBothVersions(v3_1_0, v3_1_1_p_c);

compareBothVersions(v3_1_0, v3_1_1_p);
compareBothVersions(v3_1_1, v3_1_1_p);
compareBothVersions(v3_1_1_p, v3_1_1_p);

proc compareBothVersions(v1, v2) {
  compareVersions(v1,v2);
  compareVersions(v2,v1);
}

proc compareVersions(v1, v2) {
  writeln("Comparing versions:");
  writeln(v1);
  writeln(v2);
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
  writeln(v1);
  writeln(v2);
  writeln("------------------");
  writeln("== : ", v1 == v2);
  writeln("!= : ", v1 != v2);
  writeln("<  : ", v1 < v2);
  writeln("<= : ", v1 <= v2);
  writeln();
}

proc compareGTVersions(v1, v2) {
  writeln("Comparing versions:");
  writeln(v1);
  writeln(v2);
  writeln("------------------");
  writeln("== : ", v1 == v2);
  writeln("!= : ", v1 != v2);
  writeln(">  : ", v1 > v2);
  writeln(">= : ", v1 >= v2);
  writeln();
}
