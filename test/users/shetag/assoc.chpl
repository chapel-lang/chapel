type t = (int,int);
var td : domain(t);

const one = (1,1);
const two = (2,2);

//Scenario 1 begins
td.add(one);
td.add(two);

//The two outputs match
writeln(td);
for ind in td do
  writeln(ind);

td.remove(two);
//The two outputs match
writeln(td);
for ind in td do
  writeln(ind);

td.remove(one);
//The two outputs match
writeln(td);
for ind in td do
  writeln(ind);
//Scenario 1 ends

//Scenario 2 begins
td.add(one);
td.add(two);

//The two outputs match
writeln(td);
for ind in td do
  writeln(ind);

td.remove(one);
//The two outputs *do not* match
writeln(td);
for ind in td do
  writeln(ind); 

td.remove(two);
//The two outputs match
writeln(td);
for ind in td do
  writeln(ind);
//Scenario 2 ends
