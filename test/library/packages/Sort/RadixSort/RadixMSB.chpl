 
 module RadixMSB {
   use BitOps;
   use Sort;
   use Random;
   config param bucketBits=8; 

   proc radixSortMSB(array:[?dom] int) {

     var auxArray:[1..array.size] int;
     var place = findPlace(array);
     __radixSortMSB(array,place,dom.low,dom.high,auxArray);

   }
  
   private proc __radixSortMSB(array:[] int, place: int, startIndex: int, endIndex: int, auxArray:[] int) {

     // comparison sorting for <=100 elements
     if(array.size<=100) {
       InsertionSort.insertionSort(array[startIndex..endIndex]);
       return;
     }
     
     param numBuckets = (1 << bucketBits);
     var counts:[0..numBuckets] int;
     var curOffsets: [0..numBuckets] int; 
   
     //base case, exit condition
     if (endIndex<=startIndex || place<0) {
       return;
     }
   
     for i in startIndex..endIndex {
       // find the postion to store a particular number in the counts array
       var currentDigit = findPosition(place, array[i], numBuckets);
     
       //increment count at that position
       counts[currentDigit] += 1; //increment count at the position of the m.s.d.
     }
    
     //update curOffsets
     curOffsets[0] = 0;
     for i in 1..numBuckets {
       curOffsets[i] = curOffsets[i-1] + counts[i-1];
     }


     //fill in the auxarray at correct positions 
     for i in startIndex..endIndex {
       var countIndex = findPosition(place, array[i], numBuckets);
       var auxIndex = curOffsets[countIndex];     
       auxArray[auxIndex+1] = array[i];   
       curOffsets[countIndex] += 1;                 
     }
   
     //swap slices of array and aux array   
     array[startIndex..endIndex] <=> auxArray[1..(endIndex-startIndex)];
 

     //recursive call for subparts of array
     __radixSortMSB(array, place-bucketBits, startIndex, startIndex + curOffsets[0] -1, auxArray);
     for m in 0..numBuckets-1 {        
       __radixSortMSB(array, place-bucketBits, startIndex + curOffsets[m] , startIndex + curOffsets[m+1] - 1, auxArray);
         
     }	
  	
   }

   // To find a position where an element's count is to be updated in the counts array.
   private proc findPosition(place:int, element:int, numBuckets:int) {
     var position = ((element>>place) & (numBuckets-1));
     return position;
   }

   // To find maximum element in an array
   private proc findMaxElement(array:[] int) {
     return max reduce array;
   }

   // To find the place value from which msb sorting starts
   private proc findPlace(array:[] int) {

     const maxEl = findMaxElement(array);    
     var lz = clz(maxEl);
     var numBits = 64 - lz ;
     var place = ((numBits)>>3)<<3;

     return place;
   } 
  
  }
