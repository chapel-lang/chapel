 use BitOps;
 use Sort;
 use Random;
 config param bucketBits=8; 

 module MSBmodule {

   proc radixSortMSB(array:[?dom] int) {
     var auxArray:[1..array.size] int;
     var place = findPlace(array);
     __radixSortMSB(array,place,dom.low,dom.high,auxArray);
   }
  
   private proc __radixSortMSB(array:[] int, place: int, startIndex: int, endIndex: int, auxArray:[] int) {

     param numBuckets = (1 << bucketBits);
     var counts:[1..numBuckets+1] int;  
     var currentDigit: int; 
     var countIndex: int;
     var auxIndex: int;
   
     //base case, exit condition
     if (endIndex<=startIndex || place<0) {
       return;
     }
   
     for i in startIndex..endIndex {
       // find the postion to store a particular number in the counts array
       currentDigit = findPosition(place, array[i], numBuckets);
     
       //increment count at that position
       counts[currentDigit+1] += 1; //increment count at the position of the m.s.d.
     }

     //update counts array
     counts = + scan counts;

     for i in startIndex..endIndex {
       countIndex = findPosition(place,array[i], numBuckets);
       auxIndex = counts[countIndex+1];     
       auxArray[auxIndex] = array[i];   
       counts[countIndex+1] -=1;                 
     }
  
   
     //swap slices of array and aux array   
     array[startIndex..endIndex] <=> auxArray[1..(endIndex-startIndex)];

     //recursive call for subparts of array
     for m in 1..numBuckets {
       __radixSortMSB(array, place-bucketBits, startIndex + counts[m] , startIndex + counts[m+1] - 1, auxArray);
     }	
  	
   }

   // To find a position where an element's count is to be updated in the counts array.
   private proc findPosition(place:int, element:int, numBuckets:int) {
     var position: int;
     position = ((element>>place) & (numBuckets-1));
     return position;
   }

   // To find maximum element in an array
   private proc findMaxElement(array:[] int) {
     return max reduce array;
   }

   // To find the place value from which msb sorting starts
   private proc findPlace(array:[] int) {
     var place: int;
     var lz: int;
     var maxEl: int;
     var numBits: int;

     maxEl = findMaxElement(array);    
     lz = clz(maxEl);
     numBits = 64 - lz ;
     place = ((numBits)>>3)<<3;

     return place;
   } 
  
  }
