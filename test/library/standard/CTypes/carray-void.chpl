use CTypes;

var arr: c_array(void, 5);

// The following, if uncommented, would currently lead to a poor error message,
// see carray-void.writeln.chpl
//writeln(arr);
