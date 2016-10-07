use PrivatizationWrappers;

var addOrder = [5000, 7500, 2500, 10000, 0];
var removeOrder = [5000, -1, 2500, 10000, 0];

for (i,j) in zip(addOrder, removeOrder) {
  var newValue = new C(i);
  insertPrivatized(new C(i), i);
  if j >= 0 then
    clearPrivatized(j);
}

for i in addOrder {
  writeln(getPrivatized(i));
}
