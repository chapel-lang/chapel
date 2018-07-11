class shoe {
  var size:real;
  var color:string;
  proc display() {
    writeln('Shoes:  size = ', size, '; color = ', color);
  }
}

class pumps : shoe {
  var heel: real;
  proc display() {
    writeln('Pumps:  size = ', size, '; color = ', color, '; heel ht = ', heel);
  }
}

class boots : pumps {
  proc display() {
    writeln('Boots:  size = ', size, '; color = ', color, '; heel ht = ', heel);
  }
}

proc displayshoe (item:borrowed shoe) {
  item.display();
}

var shoe1:borrowed shoe = new borrowed shoe(9.5,"brown");
var shoe2:borrowed pumps = new borrowed shoe(8.0,"navy");
shoe2.heel = 1.0;
var shoe3:borrowed boots = new borrowed pumps(7.0,"white",3.0);
displayshoe(shoe1);
displayshoe(shoe2);
displayshoe(shoe3);

writeln('shoe3.display = ');
shoe3.display();
writeln('heel ht of shoe2 = ',shoe2.heel);
