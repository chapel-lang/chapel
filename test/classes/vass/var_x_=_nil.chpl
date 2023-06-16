// test for GitHub issue #19244
// this used to crash during --no-local compilations
// this issue becomes moot if we accept the proposal in #19672

// using nil as equivalent to now-removed c_nil used in issue
var x = nil;
writeln(x);

record RR {
  var value = nil;
}

var rr: RR;
__primitive("=", rr.value, nil); // this happens upon "rr.value = nil;"
writeln(rr.value);
writeln(rr);
