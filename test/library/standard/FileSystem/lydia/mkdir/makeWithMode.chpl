use FileSystem;
// Test to ensure sent in directory permissions will be applied
config const permissions = 511;
var dirname = "useAMode";
mkdir(dirname, permissions);
