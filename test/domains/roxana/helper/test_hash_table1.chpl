class HashTable { 
  var size : integer;
  type T;
  --var size: integer;
  --var dummy : T;
  var table : _fdata(T) = _fdata(T, 20);
}  

var s : integer = 10;
  
--var ht : HashTable(T=integer) = HashTable(size=s, T=integer, dummy=0);
var ht : HashTable(T=integer) = HashTable(size=s, T=integer);
--var ht : HashTable(T=integer) = HashTable(T=integer, size=s);

for i in 1..s do {
  ht.table(i-1) = i-1;
}

/*for i in 1..ht.size do {
  if(ht.table(i-1) != ht.dummy) {
    writeln(ht.table(i-1));
  }
}*/

for i in 1..s do {
  writeln(ht.table(i-1));
}