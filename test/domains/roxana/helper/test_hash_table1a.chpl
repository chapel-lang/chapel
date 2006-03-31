class HashTable { 
  type T;
  var size: int;
  var dummy : T;
  var table : _ddata(T) = _ddata(T, 20);
}  

var s : int = 10;
  
var ht : HashTable(T=int) = HashTable(T=int, size=s, dummy=0);

for i in 1..s do {
  ht.table(i-1) = i-1;
}

/*for i in 1..s do {
  writeln(ht.table(i-1));
}*/

for i in 1..ht.size do {
  if(ht.table(i-1) != ht.dummy) {
    writeln(ht.table(i-1));
  }
}
