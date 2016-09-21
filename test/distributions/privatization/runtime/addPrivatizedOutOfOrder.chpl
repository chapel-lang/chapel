use PrivatizationWrappers;

var addOrder = [5000, 7500, 2500, 10000, 0];

for i in addOrder {
  var newValue = new C(i);
  insertPrivatized(new C(i), i);
}

for i in addOrder {
  writeln(getPrivatized(i).i);
}
