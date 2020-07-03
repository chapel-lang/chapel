use LinkedLists;

var mylist:LinkedList(int);

mylist.append(10);
mylist.append(12);
mylist.append(10);
mylist.append(12);

writeln("Your List is: ");

for i in mylist.these(){
	write(i, " ");
}
writeln("");

writeln("Accessing list by index using the .these() iterator.");
writeln(mylist.these(2));
