
var AdjD : domain sparse (VertexD * VertexD) = nil;

var weights [AdjD] : seq of wtype;

fun adjMatrix [i:AdjD.index] { return weights(i).length; }

