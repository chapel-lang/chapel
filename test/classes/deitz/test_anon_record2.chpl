var a : record {
          var x : int;
          var y : int;
          function print() {
            writeln(x, " and ", y);
          }
        };

a.x = 12;
a.y = 15;
a.print();
