class HashTable { 
  var size : integer;
  --var table : _data(integer, size);
  var table : _data(integer, 0);
  
  function Init(s : integer) {
    size = s;
    for i in 1..size do
      table(i) = 0;
  }
}  

var size : integer = 10;
  
var ht : HashTable = HashTable();
ht.size = size;
ht.table.alloc(size, "allocated table");
ht.Init(size);
writeln(ht);