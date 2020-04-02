
/*
This test is included for resolving issue #15388.

This test checks the printing of the bag's elements via a channel.
*/

use DistributedBag;

var bag = new DistBag(int);

bag.add(1..2);
writeln(bag);  //checking for multiple values in the bag
bag.add(3);
bag.removeBulk(3); 
writeln(bag);  //checking for 0 values in the bag
bag.add(5);
writeln(bag);  //checking for single value in the bag
