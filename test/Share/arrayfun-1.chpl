
var AdjD : domain sparse (VertexD * VertexD) = nil;

var weights [AdjD] : seq of wtype;

proc adjMatrix [i:AdjD.index] { return weights(i).length; }

