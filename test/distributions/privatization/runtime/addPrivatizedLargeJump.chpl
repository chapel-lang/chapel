use PrivatizationWrappers;

var addOrder = [0, 1000, 10000];

for i in addOrder {
  var newValue = new C(i);
  insertPrivatized(new C(i), i);
}

for i in addOrder {
  writeln(getPrivatized(i).i);
}

