writeln(iodynamic);
writeln(ionative);
writeln(iobig);
writeln(iolittle);
//writeln(stringStyleTerminated(b"\0"));
writeln(stringStyleNullTerminated());
writeln(stringStyleExactLen(3));
writeln(stringStyleWithVariableLength());
writeln(stringStyleWithLength(2));
writeln(IOHINT_NONE);
writeln(IOHINT_RANDOM);
writeln(IOHINT_SEQUENTIAL);
writeln(IOHINT_CACHED);
writeln(IOHINT_PARALLEL);

writeln(defaultIOStyle());
writeln(iohints: string);

//{
//  use IO only iomode;

//  var f = open("foo.txt", iomode.r);
//  writeln(f.home);
//}
//openplugin?
var f1 = openfd(0);
writeln(f1.home);

var _f2 = chpl_cstdout();
var f2 = openfp(_f2);
writeln(f2.home);
/* Things left to test
openwriter
<~> - 4 versions
stringify

stdin
stdout
stderr
stdinInit // TODO: document?
stdoutInit // TODO: document?
stderrInit // TODO: document?
read - 2 versions
readln - 3 versions
unlink
unicodeSupported
readf - 2 versions
string.format

// Haven't handled most methods yet
// I suspect those should work, it's
// just the enum constants that would
// be problematic?

// Skipped everything that was private
// for obvious reasons
// Skipped "no doc" except for when
// they were overloads of things that
// were documented
*/
