use Version;

var x = 2, y = 1, z = 3, q = 0;
var a_s = "aaa";
var b_s = "bbb";

const v2_1_0 = createProgramVersion(x,y);
const v2_1_0_a = createProgramVersion(x,y,q,a_s);
const v2_1_0_b = createProgramVersion(x,y,commit=b_s);
const v2_0_0_a = createProgramVersion(x,q,q,a_s);



compareBothVersions(v2_1_0, v2_1_0_a);
compareBothVersions(v2_0_0_a, v2_1_0_a);
compareBothVersions(v2_0_0_a, v2_1_0_b);


try! {
  writeln("<  : ", v2_1_0_b < v2_1_0_a);
} catch ex : VersionComparisonError {
  writeln(ex.message());
}

try! {
  writeln("<= : ", v2_1_0_b <= v2_1_0_a);
} catch ex : VersionComparisonError {
  writeln(ex.message());
}

try! {
  writeln(">  : ", v2_1_0_b > v2_1_0_a);
} catch ex : VersionComparisonError {
  writeln(ex.message());
}

try! {
  writeln(">= : ", v2_1_0_b >= v2_1_0_a);
} catch ex : VersionComparisonError {
  writeln(ex.message());
}


proc compareBothVersions(v1, v2) {
  try {
    compareVersions(v1,v2);
  } catch ex : VersionComparisonError {
    writeln(ex.message());
    writeln();
  } catch ex: Error {
    assert(false);
  }

  try {
    compareVersions(v2,v1);
  } catch ex : VersionComparisonError {
    writeln(ex.message());
    writeln();
  } catch ex: Error {
    assert(false);
  }

}

proc compareVersions(v1, v2) throws {
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

