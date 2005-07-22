class HashTable { 
  var size : integer;
  var table : _data(integer, size);
  
  function Init(s : integer) {
    size = s;
    for i in 1..size do
      table(i) = 0;
  }
}  

var size : integer = 10;
  
var ht : HashTable = HashTable();
ht.Init(size);
writeln(ht);