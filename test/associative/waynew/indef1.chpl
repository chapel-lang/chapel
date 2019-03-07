// Test integer indefinite domain lookup
var intd: domain(int);
intd.add(2);
intd.add(3);
intd.add(4);
intd.add(5);

proc icheck( i:int) {
  writeln( "contains(", i, ") = ", intd.contains( i));
}

icheck(0);
icheck(2);
icheck(53);
icheck(92);
icheck(5);
icheck(4);


// Test string indefinite domain lookup
var stringd: domain(string);
stringd.add( "pete");
stringd.add( "beet");
stringd.add( "cleat");
stringd.add( "defeat");
stringd.add( "eat");
stringd.add( "feet");
stringd.add( "greet");
stringd.add( "meat");

proc scheck( s:string) {
  writeln( "contains(", s, ") = ", stringd.contains( s));
}

scheck( "meet");
scheck( "peet");
scheck( "pete");
scheck( "seat");
scheck( "feat");
scheck( "feet");
