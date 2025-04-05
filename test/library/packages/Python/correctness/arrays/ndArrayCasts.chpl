use Python;


proc main() {

  var interp = new Interpreter();

  var array = interp.importModule('array').get('array');

  writeln("cast array to array");
  {
    var arr = array(owned PyArray, 'l', [1, 2, 3, 4, 5]);
    writeln(arr.type:string, arr, sep=" ");
    var arr2 = arr:owned PyArray;
    writeln(arr2.type:string, arr2, sep=" ");
  }

  writeln("cast array to array change eltType");
  {
    var arr = array(owned PyArray, 'l', [1, 2, 3, 4, 5]);
    writeln(arr.type:string, arr, sep=" ");
    var arr2 = arr:owned PyArray(int);
    writeln(arr2.type:string, arr2, sep=" ");
  }

  writeln("cast array to array change rank");
  {
    var arr = array(owned PyArray, 'l', [1, 2, 3, 4, 5]);
    writeln(arr.type:string, arr, sep=" ");
    var arr2 = arr:owned PyArray(rank=1);
    writeln(arr2.type:string, arr2, sep=" ");
  }

  writeln("cast specific array to generic array");
  {
    var arr = array(owned PyArray(int, 1), 'l', [1, 2, 3, 4, 5]);
    writeln(arr.type:string, arr, sep=" ");
    var arr2 = arr:owned PyArray;
    writeln(arr2.type:string, arr2, sep=" ");
  }

}

