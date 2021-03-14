type t = (int,int);
var td : domain(t);
var s : [td] real;

const one = (1,1);
const two = (2,2);

//Scenario 1 begins
td.add(one);
s(one) = 1.0;
td.add(two);
s(two) = 2.0;

//The two outputs match
writeln("A  ", s);
for ele in s do
  writeln("A ", ele);

td.remove(two);
//The two outputs match
writeln("B  ", s);
for ele in s do
  writeln("B ", ele);

td.remove(one);
//The two outputs match
writeln("C  ", s);
for ele in s do
  writeln("C ", ele);
//Scenario 1 ends

//Scenario 2 begins
td.add(one);
s(one) = 1.0;
td.add(two);
s(two) = 2.0;

//The two outputs match
writeln("D  ", s);
for ele in s do
  writeln("D ", ele);

td.remove(one);
//The two outputs *do not* match
writeln("E  ", s);
for ele in s do
  writeln("E ", ele); 

td.remove(two);
//The two outputs match
writeln("F  ", s);
for ele in s do
  writeln("F ", ele);
//Scenario 2 ends
