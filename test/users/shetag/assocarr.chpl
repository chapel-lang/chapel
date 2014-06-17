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
writeln(s);
for ele in s do
  writeln(ele);

td.remove(two);
//The two outputs match
writeln(s);
for ele in s do
  writeln(ele);

td.remove(one);
//The two outputs match
writeln(s);
for ele in s do
  writeln(ele);
//Scenario 1 ends

//Scenario 2 begins
td.add(one);
s(one) = 1.0;
td.add(two);
s(two) = 2.0;

//The two outputs match
writeln(s);
for ele in s do
  writeln(ele);

td.remove(one);
//The two outputs *do not* match
writeln(s);
for ele in s do
  writeln(ele); 

td.remove(two);
//The two outputs match
writeln(s);
for ele in s do
  writeln(ele);
//Scenario 2 ends
