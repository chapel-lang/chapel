use Python;



proc main() {

  var interp = new Interpreter();
  var m = new Module(interp, 'nestedArrays');
  var strNested = new Function(m, "strNested");

  {
    var input = [[1, 2], [3, 4]];
    var arr = new Array(interp, input);
    var s = strNested(string, arr, 2);
    writeln(s, "\n");
  }
  {
    var input = [[1, 2, 3, 4, 5, 6],
                 [7, 8, 9, 10, 11, 12],
                 [13, 14, 15, 16, 17, 18],
                 [19, 20, 21, 22, 23, 24],
                 [25, 26, 27, 28, 29, 30]];
    var arr = new Array(interp, input);
    var s = strNested(string, arr, 2);
    writeln(s, "\n");
  }

  {
    var input = [[[1, 2], [3, 4]],
                  [[5, 6], [7, 8]]];
    var arr = new Array(interp, input);
    var s = strNested(string, arr, 3);
    writeln(s, "\n");
  }

  {
    var input = [[[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]],
                  [[13, 14, 15, 16], [17, 18, 19, 20], [21, 22, 23, 24]],
                  [[25, 26, 27, 28], [29, 30, 31, 32], [33, 34, 35, 36]]];
    var arr = new Array(interp, input);
    var s = strNested(string, arr, 3);
    writeln(s, "\n");
  }

}
