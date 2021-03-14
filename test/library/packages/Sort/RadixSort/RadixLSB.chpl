
 module RadixLSB {
  use BitOps;
  use Sort;
  use Random;
  config param bucketBits=8;

  proc countsort(place:int,array:[] int) {

   param numBuckets = (1 << bucketBits);
   var result:[1..array.size] int;
   var counts:[0..numBuckets] int;
   var curOffsets: [0..numBuckets] int; 
   var i: int;
  
   for i in array.domain {
   
    // find the postion to store a particular number in the counts array
    var currentDigit = findPosition(place,array[i],numBuckets); 
   
    //increment count at that position
    counts[currentDigit] += 1;
   }


   //update curOffsets
   curOffsets[0] = 0;
   for i in 1..numBuckets {
     curOffsets[i] = curOffsets[i-1] + counts[i-1];
   }

   i = array.size;

   // fill in the result array with the correct position of the numbers using the curoffsets array
   for i in array.domain {
     var currentDigit = findPosition(place,array[i],numBuckets); 
     var newIndex = curOffsets[currentDigit];
     result[newIndex+1] = array[i];
     curOffsets[currentDigit] +=  1;
     //i = i - 1;
   }

   return result;
  }

 // To find maximum element in an array

  proc findMaxElement(array:[] int) {
    return max reduce array;
  }

  private proc findPosition(place:int, element:int, numBuckets:int) {
    var position = ((element>>place) & (numBuckets-1));
    return position;
  }


  private proc findPlace(array:[] int) {

    const maxEl = findMaxElement(array);    
    var lz = clz(maxEl);
    var numBits = 64 - lz ;
    var place = ((numBits)>>3)<<3;

    return place;
  } 

  proc radixSort(array:[] int) {

   var j: int;
   var place = findPlace(array);

   while(j<=place) {
     array = countsort(j,array);
     j=j+bucketBits; 
   }

   return array;
   }
 }

