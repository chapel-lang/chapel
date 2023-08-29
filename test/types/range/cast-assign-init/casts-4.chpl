// tryCast() into range(bool) is disabled, according to:
//   https://github.com/chapel-lang/chapel/issues/22905#issuecomment-1679855798
//
// When it becomes enabled, enable its testing in casts-2.chpl, casts-3.chpl
// see 22905 in comments there.

var r = 1..10 by 1000;
writeln(r.tryCast(range(bool)));
