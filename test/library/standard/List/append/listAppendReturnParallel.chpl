use List;


// Initializing
config type listType = int;
config param listLock = true;

config const stop = 10;

var lst = new list(listType, listLock);

// create a check array to the index return corresponding to the value
var checkArr : [1..stop] int;

coforall i in 1..stop with (ref lst, ref checkArr) {
  var x = i:listType;
  var ind = lst.pushBack(x);
  checkArr[i] = ind; 
}

var flg : bool = true;

// check each index stores for the value matches to the index in the list
for i in 0..<lst.size{
  if checkArr[lst.getValue(i)] != i then flg = false;
}

// print Success/Failure based on the flg
if flg then writeln("Success");
else writeln("Failure");
