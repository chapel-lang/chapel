class HashTable { 
  var size : integer;
  --var table : _data(integer, size);
  var table : _data(integer) = _data(integer, 20);
  
  function Init(s : integer) {
    size = s;
    for i in 1..size do
      table(i-1) = 0;
  }
}  

var size : integer = 10;
  
var ht : HashTable = HashTable();
ht.size = size;
ht.Init(size);
--writeln(ht);
for i in 1..size do {
  writeln(ht.table(i-1));
}
