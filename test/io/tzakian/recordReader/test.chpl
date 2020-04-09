use RecordParser, IO;

// Allow test to run with mmap or without since this test has exposed
// QIO errors in the past for one of these configurations.
config const no_mmap=false;
var hints=IOHINT_NONE;
if no_mmap {
  hints = QIO_METHOD_PREADPWRITE;
}

var f = open("input1.txt", iomode.rw);
var ff = open("input2_beer.txt", iomode.rw, hints=hints);
var fr = f.reader();
var ffr = ff.reader();

record Bar {
  var beer: string;
  var Name: string;
}

var M = new unmanaged RecordReader(Bar, fr);

writeln("======== test of get() ============");
var c = M.get();
var d = M.get();
writeln(c, " ", d);

writeln("========== test of stream() ==============");
for r in M.stream() do
  writeln(r);

record Beer {
  var name:  string;
  var beerId: int;
  var brewerId: int;
  var ABV: real;
  var style: string;
  var appearance: real;
  var aroma: real;
  var palate: real;
  var taste: real;
  var overall: real;
  var time:  int;
  var profileName:  string;
  var text: string;
}

var strt = "\\s*beer/name: (.*)\\s*beer/beerId: (.*)\\s*beer/brewerId: (.*)\\s*beer/ABV: (.*)\\s*beer/style: (.*)\\s*review/appearance: (.*)\\s*review/aroma: (.*)\\s*review/palate: (.*)\\s*review/taste: (.*)\\s*review/overall: (.*)\\s*review/time: (.*)\\s*review/profileName: (.*)\\s*review/text: (.*)";


delete M;


var N = new unmanaged RecordReader(Beer, ffr, strt);

writeln("========== test of stream() ==============");
for r in N.stream() do
  writeln(r);

delete N;
