var a : record {
          var x : int;
          var y : int;
          def print() {
            writeln(x, " and ", y);
          }
        };

a.x = 12;
a.y = 15;
a.print();
