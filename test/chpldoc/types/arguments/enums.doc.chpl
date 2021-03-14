/* This is a color enum. */
enum Color { Red, Yellow, Blue };

/* This function takes an enum as an argument */
proc gimmeAColor(c: Color) {

}

/* This function makes an enum the default value for
   its argument
*/
proc colorDefault(c = Color.Red) {

}

/* This function has both declared that it wants an enum
   and defined a default value for its argument */
proc defaultAndType(c: Color = Color.Blue) {

}
