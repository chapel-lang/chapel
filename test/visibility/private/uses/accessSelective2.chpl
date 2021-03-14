use M; // from selectiveUses.chpl

writeln(a);
// Verifies that a pair of private/public uses of the same module don't cause
// the public use to be mishandled.  Should find 'a' without issue
