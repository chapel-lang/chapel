def _domain.callNewDist() {
  var x = new dist(defaultDist._value);
  return x;
}

def callNewDistOutsideDomain(val) {
  var x = new dist(val);
  return x;
}


var D = [1..10];
D.callNewDist();
