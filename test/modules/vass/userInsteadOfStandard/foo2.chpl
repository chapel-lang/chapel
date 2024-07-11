// This test and ./ChapelIO.chpl are copies of those in
//   test/modules/bradc/userInsteadOfStandard/foo2.chpl
// with a tweak to ./ChapelIO.chpl to illustrate a bug.
// It also differs from that test because it uses
//  --prepend-standard-module-dir .
// (because otherwise ./ChapelIO.chpl is not used at all)
use ChapelIO;

testchapelio();
writeln("In my foo2");
compilerError("done");
