use HDFSiterator;

//// ---------- Test/Example usage --------
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

var strt = "beer/name: (.*)\\s*beer/beerId: (.*)\\s*beer/brewerId: (.*)\\s*beer/ABV: (.*)\\s*beer/style: (.*)\\s*review/appearance: (.*)\\s*review/aroma: (.*)\\s*review/palate: (.*)\\s*review/taste: (.*)\\s*review/overall: (.*)\\s*review/time: (.*)\\s*review/profileName: (.*)\\s*review/text: (.*)\\n\\n";

var apps:     atomic real;
var aromas:   atomic real;
var palates:  atomic real;
var tastes:   atomic real; 
var overalls: atomic real; 
var total:    atomic int(64);
total.write(0);


// var summary = beer reduce HDFSiter("/tmp/advocate.txt", Beer, strt);

writeln("========== Starting test ==============");
forall r in HDFSiter("/tmp/beer.txt", Beer, strt) {
 total.add(1);
 apps.add(r.appearance);
 aromas.add(r.aroma);
 palates.add(r.palate);
 tastes.add(r.taste);
 overalls.add(r.overall);
}

writeln("\n\nDone reading.");
writeln("\n\ntotal number of records read is: ", total.read());
writeln("average app: ", apps.read()/total.read());
writeln("average aroma: ", aromas.read()/total.read());
writeln("average palate: ", palates.read()/total.read());
writeln("average taste: ", tastes.read()/total.read());
writeln("average overall: ", overalls.read()/total.read());

