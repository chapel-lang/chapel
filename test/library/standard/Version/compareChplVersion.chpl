use Version;

var zero = 0;
var one = 1;
var two = 2;
var three = 3;


const v2_1_0c = createVersion(two,one,zero);
const v2_1_1c = createVersion(two,one,one);
const v2_2_0c = createVersion(two,two,zero);
const v2_2_1c = createVersion(two,two,one);
const v3_1_0c = createVersion(three,one);
const v3_1_1c = createVersion(three,one,one);

const v2_1_0 = createVersion(2,1);
const v2_1_1 = createVersion(2,1,1);
const v2_2_0 = createVersion(2,2,0);
const v2_2_1 = createVersion(2,2,1);
const v3_1_0 = createVersion(3,1);
const v3_1_1 = createVersion(3,1,1);

const v2_1_0_pc = createVersion(two,one,zero,"aaa");
const v2_1_1_pc = createVersion(two,one,one,"bbb");
const v2_2_0_pc = createVersion(two,two,commit="ccc");
const v2_2_1_pc = createVersion(two,two,one,"ddd");
const v3_1_0_pc = createVersion(three,one,zero,"eee");
const v3_1_1_pc = createVersion(three,one,one,"fff");

const v2_1_0_p = createVersion(2,1,0,"aaa");
const v2_1_1_p = createVersion(2,1,1,"bbb");
const v2_2_0_p = createVersion(2,2,commit="ccc");
const v2_2_1_p = createVersion(2,2,1,"ddd");
const v3_1_0_p = createVersion(3,1,0,"eee");
const v3_1_1_p = createVersion(3,1,1,"fff");

/* when LHS sourceVersions is const and RHS is param */
writeln("="*80);
writeln("LHS is const and RHS is param style");
writeln("="*80);

compareVersions(v3_1_1c, v3_1_1);
compareVersions(v3_1_1_pc, v3_1_1_p);

compareVersions(v2_1_0c, v2_1_0);
compareVersions(v2_1_0_pc, v2_1_0_p);
compareLTVersions(v2_1_0c, v2_1_0_p);
compareGTVersions(v2_1_0_pc, v2_1_0);

compareBothVersions(v2_1_0c, v2_1_1);
compareBothVersions(v2_1_0c, v2_2_0);
compareBothVersions(v2_1_0c, v2_2_1);
compareBothVersions(v2_1_0c, v3_1_0);
compareBothVersions(v2_1_0c, v3_1_1);

compareBothVersions(v2_1_0c, v2_1_1_p);
compareBothVersions(v2_1_0c, v2_2_0_p);
compareBothVersions(v2_1_0c, v2_2_1_p);
compareBothVersions(v2_1_0c, v3_1_0_p);
compareBothVersions(v2_1_0c, v3_1_1_p);

compareBothVersions(v2_1_1c, v2_2_0);
compareBothVersions(v2_1_1c, v2_2_1);
compareBothVersions(v2_1_1c, v3_1_0);
compareBothVersions(v2_1_1c, v3_1_1);

compareBothVersions(v2_1_1c, v2_2_0_p);
compareBothVersions(v2_1_1c, v2_2_1_p);
compareBothVersions(v2_1_1c, v3_1_0_p);
compareBothVersions(v2_1_1c, v3_1_1_p);

compareBothVersions(v2_2_0c, v2_2_1);
compareBothVersions(v2_2_0c, v3_1_0);
compareBothVersions(v2_2_0c, v3_1_1);

compareBothVersions(v2_2_0c, v2_2_1_p);
compareBothVersions(v2_2_0c, v3_1_0_p);
compareBothVersions(v2_2_0c, v3_1_1_p);

compareBothVersions(v2_2_1c, v3_1_0);
compareBothVersions(v2_2_1c, v3_1_1);

compareBothVersions(v2_2_1c, v3_1_0_p);
compareBothVersions(v2_2_1c, v3_1_1_p);

compareBothVersions(v3_1_0c, v3_1_1);

compareBothVersions(v3_1_0c, v3_1_1_p);


/* when LHS sourceVersions is param and RHS is const */
writeln("="*80);
writeln("LHS is param and RHS is const style");
writeln("="*80);
compareVersions(v3_1_1, v3_1_1c);
compareVersions(v3_1_1_p, v3_1_1_pc);

compareVersions(v2_1_0, v2_1_0c);
compareVersions(v2_1_0_p, v2_1_0_pc);
compareLTVersions(v2_1_0, v2_1_0_pc);
compareGTVersions(v2_1_0_p, v2_1_0c);

compareBothVersions(v2_1_0, v2_1_1c);
compareBothVersions(v2_1_0, v2_2_0c);
compareBothVersions(v2_1_0, v2_2_1c);
compareBothVersions(v2_1_0, v3_1_0c);
compareBothVersions(v2_1_0, v3_1_1c);

compareBothVersions(v2_1_0, v2_1_1_pc);
compareBothVersions(v2_1_0, v2_2_0_pc);
compareBothVersions(v2_1_0, v2_2_1_pc);
compareBothVersions(v2_1_0, v3_1_0_pc);
compareBothVersions(v2_1_0, v3_1_1_pc);

compareBothVersions(v2_1_1, v2_2_0c);
compareBothVersions(v2_1_1, v2_2_1c);
compareBothVersions(v2_1_1, v3_1_0c);
compareBothVersions(v2_1_1, v3_1_1c);

compareBothVersions(v2_1_1, v2_2_0_pc);
compareBothVersions(v2_1_1, v2_2_1_pc);
compareBothVersions(v2_1_1, v3_1_0_pc);
compareBothVersions(v2_1_1, v3_1_1_pc);

compareBothVersions(v2_2_0, v2_2_1c);
compareBothVersions(v2_2_0, v3_1_0c);
compareBothVersions(v2_2_0, v3_1_1c);

compareBothVersions(v2_2_0, v2_2_1_pc);
compareBothVersions(v2_2_0, v3_1_0_pc);
compareBothVersions(v2_2_0, v3_1_1_pc);

compareBothVersions(v2_2_1, v3_1_0c);
compareBothVersions(v2_2_1, v3_1_1c);

compareBothVersions(v2_2_1, v3_1_0_pc);
compareBothVersions(v2_2_1, v3_1_1_pc);

compareBothVersions(v3_1_0, v3_1_1c);

compareBothVersions(v3_1_0, v3_1_1_pc);

/* when both sourceVersions are const style */
writeln("="*80);
writeln("LHS and RHS are const style");
writeln("="*80);
compareVersions(v3_1_1c, v3_1_1c);
compareVersions(v3_1_1_pc, v3_1_1_pc);

compareVersions(v2_1_0c, v2_1_0c);
compareVersions(v2_1_0_pc, v2_1_0_pc);
compareLTVersions(v2_1_0c, v2_1_0_pc);
compareGTVersions(v2_1_0_pc, v2_1_0c);

compareBothVersions(v2_1_0c, v2_1_1c);
compareBothVersions(v2_1_0c, v2_2_0c);
compareBothVersions(v2_1_0c, v2_2_1c);
compareBothVersions(v2_1_0c, v3_1_0c);
compareBothVersions(v2_1_0c, v3_1_1c);

compareBothVersions(v2_1_0c, v2_1_1_pc);
compareBothVersions(v2_1_0c, v2_2_0_pc);
compareBothVersions(v2_1_0c, v2_2_1_pc);
compareBothVersions(v2_1_0c, v3_1_0_pc);
compareBothVersions(v2_1_0c, v3_1_1_pc);

compareBothVersions(v2_1_1c, v2_2_0c);
compareBothVersions(v2_1_1c, v2_2_1c);
compareBothVersions(v2_1_1c, v3_1_0c);
compareBothVersions(v2_1_1c, v3_1_1c);

compareBothVersions(v2_1_1c, v2_2_0_pc);
compareBothVersions(v2_1_1c, v2_2_1_pc);
compareBothVersions(v2_1_1c, v3_1_0_pc);
compareBothVersions(v2_1_1c, v3_1_1_pc);

compareBothVersions(v2_2_0c, v2_2_1c);
compareBothVersions(v2_2_0c, v3_1_0c);
compareBothVersions(v2_2_0c, v3_1_1c);

compareBothVersions(v2_2_0c, v2_2_1_pc);
compareBothVersions(v2_2_0c, v3_1_0_pc);
compareBothVersions(v2_2_0c, v3_1_1_pc);

compareBothVersions(v2_2_1c, v3_1_0c);
compareBothVersions(v2_2_1c, v3_1_1c);

compareBothVersions(v2_2_1c, v3_1_0_pc);
compareBothVersions(v2_2_1c, v3_1_1_pc);

compareBothVersions(v3_1_0c, v3_1_1c);

compareBothVersions(v3_1_0c, v3_1_1_pc);

/* when both sourceVersions are param style */
writeln("="*80);
writeln("LHS and RHS are param style");
writeln("="*80);
compareVersions(v3_1_1, v3_1_1);
compareVersions(v3_1_1_p, v3_1_1_p);

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

