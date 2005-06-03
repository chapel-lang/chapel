var a : record {
          var x : integer;
          var y : integer;
          function print() {
            writeln(x, " and ", y);
          }
        };

a.x = 12;
a.y = 15;
a.print();
