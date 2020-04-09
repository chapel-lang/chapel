use recordParser;

var f = open("input1.txt", iomode.rw);
var ff = open("input2_beer.txt", iomode.rw);
var fr = f.reader();
var ffr = ff.reader();

record Bar {
  var beer: string;
  var Name: string;
}

//var str = "\\s*beer\\s*(.*?)\\s*Name\\s*(.*?)\\s*";
var str = "\\s*beer\\s*(.*?)\\s*Name\\s*(.*?)\\s*";

var M = new RecordReader(Bar, fr);

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

var N = new RecordReader(Beer, ffr, strt);
writeln("========== test of stream() ==============");
for r in N.stream() do
  writeln(r);

