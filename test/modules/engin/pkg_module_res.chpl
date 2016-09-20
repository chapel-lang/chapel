use Math; //doesn't make a difference -- works either way
writeln(Math.pi);
writeln(pi);

use Norm;
var arr: [{0..10}] int;
writeln(norm(arr));
writeln(Norm.norm(arr)); //works 

use Curl;
var sl: slist;
var sl2: Curl.slist;

use Sort;
writeln(isSorted(arr));
writeln(Sort.isSorted(arr));

use Search;
writeln(linearSearch(arr,1));

//compilation doesn't come to this point but following line also fails when run
//standalone
writeln(Search.linearSearch(arr,1));
