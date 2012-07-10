/* June 2005 -- the following code results in a use before
   def error for Vertices */
config var numVertices: int = 3;
var Vertices: domain(1) = {1..numVertices};
type vertex = index(Vertices);

writeln("If we see this, the test probably worked");
