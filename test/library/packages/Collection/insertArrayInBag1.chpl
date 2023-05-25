use DistributedBag;

var arr: [0..4] int = [0, 1, 2, 3, 4];
var bag1 = new DistBag([0..4] int, targetLocales=Locales);
bag1.add(arr);
