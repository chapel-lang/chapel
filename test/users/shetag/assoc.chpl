type t = (int,int);
var td : domain(t);

const one = (1,1);
const two = (2,2);

//Scenario 1 begins
td.add(one);
td.add(two);

//The two outputs match
writeln("A  ", td.sorted());
for ind in td do
  writeln("A ", ind);

td.remove(two);
//The two outputs match
writeln("B  ", td.sorted());
for ind in td do
  writeln("B ", ind);

td.remove(one);
//The two outputs match
writeln("C  ", td.sorted());
for ind in td do
  writeln("C ", ind);
//Scenario 1 ends

//Scenario 2 begins
td.add(one);
td.add(two);

//The two outputs match
writeln("D  ", td.sorted());
for ind in td do
  writeln("D ", ind);

td.remove(one);
//The two outputs *do not* match
writeln("E  ", td.sorted());
for ind in td do
  writeln("E ", ind); 

td.remove(two);
//The two outputs match
writeln("F  ", td.sorted());
for ind in td do
  writeln("F ", ind);
//Scenario 2 ends
