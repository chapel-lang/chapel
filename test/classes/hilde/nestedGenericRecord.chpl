// nestedGenericRecord.chpl
//
// Demonstrate the propagation of type constraints through a record definition.
//

enum Color {red, green, blue};

record Point
{
  type Coord = int;
  param fill : Color = Color.red;
  param filled : bool = false;

  var x, y : Coord;
}

record Circle
{
  type Coord = int;
  param Fill : Color = Color.blue;
  param Filled : bool = true;

  var center : Point(Coord, Fill, Filled);
  var radius : Coord;
}

var c:Circle(real, Color.green, false);

writeln(c.type:string);
