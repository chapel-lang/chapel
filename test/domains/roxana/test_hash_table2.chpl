class HashTable {
  --type T : integer;  
  var size : integer;
  var table : [1..size] integer;
  
  function Init(s : integer) {
    size = s;
    forall i in 1..size {
      table(i) = 0;
    }
  }
}  

var size : integer;
writeln("Enter table size");
read(size);
writeln("Size is: ", size);
  
var ht : HashTable = HashTable(size);
--ht.Init(size);
writeln(ht);