use DistributedMap;

record myHasher {
    proc this(k: int): int {
        return int % 5;
    }
}

var dm = new distributedMap(string, int);
var dm_withhasher = new distributedMap(int, string, new myHasher());
