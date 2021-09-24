use MasonExternal;

var v1 = "0.15.0";
var v2 = "0.16.0";
var v3 = "0.15.1";
var v4 = "1.15.0";
var v5 = "1.0.0";
var v6 = "0.0.9";

// same major, lower minor, same patch
// should output -1
writeln(compareSpackVersion(v1,v2):string);

// same major, same minor, lower patch
// should output -1
writeln(compareSpackVersion(v1,v3):string);

// lower major, same minor, same patch
// should output -1
writeln(compareSpackVersion(v1,v4):string);

// same major, minor, patch
// should output 0
writeln(compareSpackVersion(v1,v1):string);

// higher major, lower minor, lower patch
// should output 1
writeln(compareSpackVersion(v4,v1):string);

// same major, higher minor, same patch
// should output 1
writeln(compareSpackVersion(v2,v1):string);

// same major, same minor, higher patch
// should output 1
writeln(compareSpackVersion(v3,v1):string);

// higher major, same minor, lower patch
// should output 1
writeln(compareSpackVersion(v5,v6):string);

// lower major, same minor, higher patch
// should output -1
writeln(compareSpackVersion(v6,v5):string);

