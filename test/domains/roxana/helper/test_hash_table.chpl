--param del = -1000000;
--var del = -1000000;

class HashTable {
  type T;  
  var size : int;
  var del : T;
  var null : T;
   
  var table: _fdata(T) = _fdata(T, 20);
    
  function h1(k : int) : int {
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
  
  function h2(k : int) : int {
    return (1 + k mod (size - 1));
  }
  
  function h(k : int, i : int) : int {
    return ((h1(k) + i*h2(k)) mod size);
  }
  
  function Insert(k: int, v : T) : int {
    var j : int = -1;
    var i : int = 0;
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
  
  function Remove(k : int) : HashTable {
    var j : int = Search(k);
    if (j == -1) {
      writeln("The key you want to delete is not in the table!");
      return this;
    }
    table(j) = del;
    return this;
  }
  
  function Search (k : int) : int {
    var i : int = 0;
    var j : int = -1;
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

  --var size : int;
  --writeln("Enter table size");
  --read(size);
  --writeln("Size is: ", size);
  
  var s: int = 10;
  
  var del : int;
  writeln("Enter a <deleted key> value.");
  read(del);
  
  var null : int;
  writeln("Enter a <null key> value.");
  read(null);
  
  var ht : HashTable(T=int) = HashTable(T=int, size=s, del=del, null=null);
  
  ht.Init();
  
  var i : int = 0;
  var v : int;
  
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
