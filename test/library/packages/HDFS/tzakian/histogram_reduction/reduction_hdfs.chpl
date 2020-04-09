use HDFSiterator;

record Beer {
  var name:  string;
  var beerId: int;
  var brewerId: int;
  var ABV: real;
  var style: string;
  var appearance: real;
  var aroma, palate, taste, overall: real;
 // var palate: real;
 // var taste: real;
 // var overall: real;
  var time:  int;
  var profileName:  string;
  var text: string;
}

var strt = "beer/name: (.*)\\s*beer/beerId: (.*)\\s*beer/brewerId: (.*)\\s*beer/ABV: (.*)\\s*beer/style: (.*)\\s*review/appearance: (.*)\\s*review/aroma: (.*)\\s*review/palate: (.*)\\s*review/taste: (.*)\\s*review/overall: (.*)\\s*review/time: (.*)\\s*review/profileName: (.*)\\s*review/text: (.*)\\n\\n";

param NBUCKETS = 5;
var max:int(64) = 6;
var min:int(64) = 0;

class myhisto: ReduceScanOp {
  type eltType;
  const per = (max - min)/NBUCKETS;
  var counts:[0..NBUCKETS] int;

  proc accumulate(x) {
    counts[(round(x.overall):int)/per] += 1;
  }
  proc combine(x) {
    for i in 1..NBUCKETS {
      counts[i] += x.counts[i];
    }
  }
  proc generate() {
    return counts;
  }
}

var counts = myhisto reduce HDFSiter("/tmp/advocate.txt", Beer, strt);

writeln(counts);

var t = max reduce counts;
writeln(t);

var A = [0,1,2,3,4,5,6,1,2,3,4];

//printGraph(counts, t, 5);

proc printGraph(arr: [] int, cols: int, max: int) {
  for r in 1..max {
    for c in 1..cols {
      if(r+arr[c] >= max) 
        then write("\x1b[34m.\x1b[0m");
      else write(" ");
    }
    writeln();
  }
}


