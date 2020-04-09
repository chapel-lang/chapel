use PrivatizationWrappers;

var addOrder = [5000, 7500, 2500, 10000, 0];
var removeOrder = [5000, -1, 2500, 10000, 0];

for (i,j) in zip(addOrder, removeOrder) {
  var newValue = new unmanaged C(i);
  insertPrivatized(newValue, i);
  if j >= 0 {
    var c = getPrivatized(j);
    delete c;
    clearPrivatized(j);
  }
}

for i in addOrder {
  writeln(getPrivatized(i));
}

for i in addOrder {
  var c = getPrivatized(i);
  delete c;
  clearPrivatized(i);
}
