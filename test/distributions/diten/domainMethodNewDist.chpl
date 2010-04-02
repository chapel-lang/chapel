def _domain.callNewDist() {
  var x = new dmap(defaultDist._value);
  return x;
}

def callNewDistOutsideDomain(val) {
  var x = new dmap(val);
  return x;
}


var D = [1..10];
D.callNewDist();
