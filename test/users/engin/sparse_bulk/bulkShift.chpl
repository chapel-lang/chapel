use LayoutCSR;

const D = {0..#10, 0..#10};

/*var toAdd = [(2,2), (3,3)];*/
var toAdd: [{0..#2}] 2*int;

toAdd[0] = (2,2);
toAdd[1] = (3,3);

var toAdd2: [{0..#2}] 2*int;

toAdd2[0] = (2,4);
toAdd2[1] = (1,1);

var SD: sparse subdomain(D);// dmapped new dmap(new CSR());
var arr: [SD] int;

SD.add((0,0));

arr[0,0] = 5;

SD.bulkAdd(toAdd);

arr[2,2] = 10;

SD.bulkAdd(toAdd2);

arr[2,4] = 25;
arr[1,1] = 7;

for a in arr do writeln(a);
for i in SD do writeln(i, " ", arr[i]);

