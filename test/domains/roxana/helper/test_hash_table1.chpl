class HashTable { 
  var size : integer;
  type T;
  var dummy : T;
  var table : _data(T) = _data(T, 20);
}  

var size : integer = 10;
  
var ht : HashTable = HashTable(integer, size, 0);

for i in 1..ht.size do {
  ht.table(i-1) = i-1;
}

for i in 1..ht.size do {
  if(ht.table(i-1) == ht.dummy) {
    writeln(ht.table(i-1));
  }
}
