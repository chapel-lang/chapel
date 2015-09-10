/* This is a color enum. */
enum Color { Red, Yellow, Blue };

/* This class holds many different colors */
class colorCup {
  /* Field has only enum as a default value */
  var defOnly = Color.Yellow;
  /* Field has only enum as its type */
  var typeOnly: Color;
  /* Field has both a type and default value */
  var both: Color = Color.Red;
}
