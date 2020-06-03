use M; // from selectiveUses.chpl

writeln(c);
// Verifies that a pair of private/public uses of the same module don't cause
// the private use to be mishandled.  Shouldn't find 'c'
