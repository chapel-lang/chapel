record pants{
  var size:int;
  var color:string;
  proc display() {
    writeln('Pants:  size = ', size, '; color = ', color);
  }
}

record jeans: pants{
  var rise:string;
  proc display() {
    writeln('Jeans:  size = ', size, '; color = ', color, '; rise = ',
rise);
  }
}


proc displaypants (item:pants) {
  item.display();
}

var pant1:jeans = new jeans(size = 6, color = "dark wash",rise = "low rise");
displaypants(pant1);
