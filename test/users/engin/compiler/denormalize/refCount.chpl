var dom ={0..10};
/*var arr: [dom] int;*/

/*for i in dom do arr[i] = -i;*/
/*sort(arr);*/
var sdom: sparse subdomain(dom);
var d = {0..1};
var a: [d] int;
a[0] = 3;
a[1] = 4;
writeln(a.domain._value._domCnt.read());
sdom += a;
/*sdom.bulkAdd(a, preserveInds=false);*/
writeln(a.domain._value._domCnt.read());
