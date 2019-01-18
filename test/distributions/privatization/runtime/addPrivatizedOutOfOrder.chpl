use PrivatizationWrappers;

var addOrder = [5000, 7500, 2500, 10000, 0];

for i in addOrder {
  var newValue = new unmanaged C(i);
  insertPrivatized(newValue, i);
}

for i in addOrder {
  writeln(getPrivatized(i).i);
}

// no leaks
for i in addOrder {
  var c = getPrivatized(i);
  delete c;
  clearPrivatized(i);
}
