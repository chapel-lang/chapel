use BlockDist;

var WorkSpace = {1..10} dmapped Block(boundingBox={1..10});
const WorkArr : [WorkSpace] [1..5] int;

forall b in WorkArr {
	for el in b {
		el = 1;
	}
}
