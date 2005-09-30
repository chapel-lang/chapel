--param del = -1000000;
--var del = -1000000;

class HashTable {
  type T;  
  var size : integer;
  var del : T;
  var null : T;
   
  var table: _fdata(T) = _fdata(T, 20);
    
  function h1(k : integer) : integer {
    return k mod size;
  }
  
  function Init() {
    if (null) {
      for i in 1..size do
        table(i-1) = null;
    }
    else {
      --halt
      writeln("You should have set a null value");
    }
  }    
  
  function h2(k : integer) : integer {
    return (1 + k mod (size - 1));
  }
  
  function h(k : integer, i : integer) : integer {
    return ((h1(k) + i*h2(k)) mod size);
  }
  
  function Insert(k: integer, v : T) : integer {
    var j : integer = -1;
    var i : integer = 0;
    while (i < size){
      j = h(k, i);
      if (table(j) == null or table(j) == del) {
        table(j) = v;
        return j;
      }
      i += 1;
    }
    writeln("Table overflow!");
    return -1;
  }
  
  function Remove(k : integer) : HashTable {
    var j : integer = Search(k);
    if (j == -1) {
      writeln("The key you want to delete is not in the table!");
      return this;
    }
    table(j) = del;
    return this;
  }
  
  function Search (k : integer) : integer {
    var i : integer = 0;
    var j : integer = -1;
    while ((i < size) and (table(j) != null)) {
      j = h(k, i);
      if (j == k){
        return j;
      }    
      i += 1;
    }
    writeln("Key not found.");
    return -1;
  }
}

  --var size : integer;
  --writeln("Enter table size");
  --read(size);
  --writeln("Size is: ", size);
  
  var s: integer = 10;
  
  var del : integer;
  writeln("Enter a <deleted key> value.");
  read(del);
  
  var null : integer;
  writeln("Enter a <null key> value.");
  read(null);
  
  var ht : HashTable(T=integer) = HashTable(T=integer, size=s, del=del, null=null);
  
  ht.Init();
  
  var i : integer = 0;
  var v : integer;
  
  while(i < s) {
    writeln("Enter an element to insert");
    read(v);
    --if (v == ht.del)
	--  halt("You cannot insert a key with the same value as the deleted key.");
    ht.Insert(i, v);
    i += 1;
  }  
  --writeln(ht);
  
  for i in 1..ht.size do {
    writeln(ht.table(i-1));
  }