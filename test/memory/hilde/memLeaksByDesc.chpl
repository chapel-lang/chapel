// Test the --memLeaksByDesc option.

class C { var i:int; }

class D { var r:real; }

// Allocate some class objects of distinguishable type.

var c = new unmanaged C();

var d = new unmanaged D();

// And leak them here.
