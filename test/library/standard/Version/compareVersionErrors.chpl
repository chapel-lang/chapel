use Version;

var x = 2, y = 1, z = 3, q = 0;
var a_s = "aaa";
var b_s = "bbb";

var v2_1_0 = new version(x,y);
var v2_1_0_a = new version(x,y,q,a_s);
var v2_1_0_b = new version(x,y,commit=b_s);
var v2_0_0_a = new version(x,q,q,a_s);



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

