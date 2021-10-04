use MasonExternal;
use MasonUtils;

var v1 = new VersionInfo("0.15.0");
var v2 = new VersionInfo("0.16.0");
var v3 = new VersionInfo("0.15.1");
var v4 = new VersionInfo("1.15.0");
var v5 = new VersionInfo("1.0.0");
var v6 = new VersionInfo("0.0.9");

// same major, lower minor, same patch
// should output true
writeln((v1 < v2):string);

// same major, same minor, lower patch
// should output true
writeln((v1 < v3):string);

// lower major, same minor, same patch
// should output true
writeln((v1 < v4):string);

// same major, minor, patch
// should output true
writeln((v1 == v1):string);

// higher major, lower minor, lower patch
// should output false
writeln((v4 < v1):string);

// same major, higher minor, same patch
// should output true
writeln((v2 > v1):string);

// same major, same minor, higher patch
// should output true
writeln((v3 > v1):string);

// higher major, same minor, lower patch
// should output false
writeln((v5 <= v6):string);

// lower major, same minor, higher patch
// should output true
writeln((v6 <= v5):string);

// bad minor
// should be false
writeln(v1.isCompatible(v2):string);

// bad major
// should be false
writeln(v1.isCompatible(v4):string);

// bad major and minor
// should be false
writeln(v1.isCompatible(v5):string);

// bad minor
// should be false
writeln(v1.isCompatible(v6):string);

// bad minor
// should be false
writeln(v4.isCompatible(v5):string);

// bad major
// should be false
writeln(v5.isCompatible(v6):string);

// matching major, minor, lower patch
// should be false
writeln(v3.isCompatible(v1):string);

// matching major, minor, higher patch
// should be true
writeln(v1.isCompatible(v3):string);
