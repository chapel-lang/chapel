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

var f3:string;
var f4 = read(int);
writeln(f4);
var f5 = read(f3);
writeln(f5);
f4 = readln(int);
writeln(f4);
f5 = readln();
writeln(f5);
f5 = readln(f3);
writeln(f5);
f5 = readf("%i", f4);
writeln(f5);


stdinInit();    // TODO: document?
stdoutInit();   // TODO: document?
stderrInit();   // TODO: document?
{
    use IO;

    var f = open("test.txt", iomode.cw);
}
unlink("test.txt");
unicodeSupported();
/* Things left to test
openwriter
<~> - 4 versions
stringify

stdin
stdout
stderr

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
