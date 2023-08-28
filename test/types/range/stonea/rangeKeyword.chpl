// All the following should be disallowed (since we consider 'range' to be a keyword):
var range = 42;
proc range() { return 42; }
iter range() { yield 42; }
record range { }
class range { }
