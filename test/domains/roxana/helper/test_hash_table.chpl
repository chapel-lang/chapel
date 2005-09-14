param del = -1000000;

class HashTable {
  type T;  
  var size : integer;
  --var table : [1..size] T;
  var table : _data(T, size);
 
  function KeyToInt(k : T) : integer {
 	/*typeselect (T) {
 	  when k : integer do return k;
 	  otherwise return 0;
 	} */
 	return k;
  }
  
  function h1(k : T) : integer {
    k = KeyToInt(k);
    return k mod size;
  }
  
  function h2(k : T) : integer {
    k = KeyToInt(k);
    return (1 + k mod (size - 1));
  }
  
  function Init(s : integer) {
    size = s;
    [i in 1..size] table(i) = nil;
  }

  function h(k : T, i : integer) : integer {
    k = KeyToInt(k);
    return ((h1(k) + i*h2(k)) mod size);
  }
  function Insert(k : T) : integer {
    var j : integer = -1;
    var i : integer = 0;
    while (i < size){
      j = h(k, i);
      if (table(j) == nil or table(j) == del) {
        table(j) = k;
        return j;
      }
      i += 1;
    }
    writeln("Table overflow!");
    return -1;
  }
  
  function Remove(k : T) : HashTable {
    var j : integer = Search(k);
    if (j == -1) {
      writeln("The key you want to delete is not in the table!");
      return this;
    }
    table(j) = del;
    return this;
  }
  
  function Search (k : T) : integer {
    var i : integer = 0;
    var j : integer = -1;
    while ((i < size) or (table(j) == nil)) {
      j = h(k, i);
      if (table(j) == k){
        return j;
      }    
      i += 1;
    }
    writeln("Key not found.");
    return -1;
  }
}

  var size : integer;
  writeln("Enter table size");
  read(size);
  writeln("Size is: ", size);
  
  var ht : HashTable = HashTable(integer);
  /*ht.Init(size);
  
  var i : integer = 0;
  var k : integer;
  
  while(i < size) {
    writeln("Enter a key to insert");
    read(k);
    ht.Insert(k);
    i += 1;
  }  
  writeln(ht);
*/
