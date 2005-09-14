class HashTable { 
  var size : integer;
  --var table : _data(integer, size);
  --var table : _data(integer, 0);
  var table: _data(integer);
  function init() { 
    table.alloc(size, "table alloc");
  }
}  

var s1 : integer = 10;

var ht : HashTable = HashTable(s1);
--ht.size = s1;
--ht.table.alloc(s1, "allocated table");

ht.init();

for i in 1..s1 do {
  ht.table(i-1) = 0;
}

writeln(ht);
