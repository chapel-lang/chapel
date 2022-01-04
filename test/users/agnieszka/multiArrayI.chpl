/*
Program is a solution to atomic broadcast problem for multiple locales.

As a data storage(to write/read the product) array is used
(containing numLocales*4-1 elements, one for each Customer).

Array's elements are also used for synchronization (as sync variables).

Product is put into all array elements (for all locales being Customers).
As every element is a sync variable (at the beginning logically empty),
it can be only written at first (what makes it logically full).
It cannot be written again until it is read by appropriate
Customer-locale (what makes it logically empty).
*/


multi_array();


// P - number of products

proc multi_array(P: int=100) {
  // Domain -  Consumers
  var D: domain(1) = {1..numLocales*4-1};
  // elements of array A used to write/read
  // the product and for synchronization
  var A$: [D] sync int;

  cobegin {
    // Producer
    on Locales(0) {
      var put = 1;
      for i in 1..P {
        // put product in all array elements
        forall k in D do
          A$(k).writeEF(put);

        // writeln("Put: ",put);
        // create new product
        put += 1;
      }
    }
    // Consumers
    coforall j in D do on Locales(j%numLocales) {
      var  get: int;
      for i in 1..P {
        // read the product
        get = A$(j).readFE();
      }
    }
  }
}
