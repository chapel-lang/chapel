class HashTable { 
  var size : integer;
  var table : _data(integer, size);
}  

var s1 : integer = 10;

var ht : HashTable = HashTable();
ht.size = s1;
for i in 1..s1 do {
  ht.table(i) = 0;
}

writeln(ht);
