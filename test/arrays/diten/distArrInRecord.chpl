use BlockDist;
const blkDom = {1..1} dmapped new Block(boundingBox={1..1});
record R { var Arr: [blkDom] int; }
