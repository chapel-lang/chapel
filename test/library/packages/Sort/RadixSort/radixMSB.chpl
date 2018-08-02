use BitOps;
use Sort;
use Random;

config param bucketBits=8; 

 module MSBmodule{

  proc radixSortMSB(array:[] int, place: int, startIndex: int, endIndex: int){
    var auxArray:[1..array.size] int ;
    radix_sort_msd(array,place,1,array.size,auxArray);
  }
  
  proc radix_sort_msd(array:[] int, place: int, startIndex: int, endIndex: int, auxArray:[] int) {

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
      radix_sort_msd(array, place-bucketBits, startIndex + counts[m] , startIndex + counts[m+1] - 1, auxArray);
    }	
  	
  }

   // To find a position where an element's count is to be updated in the counts array.
  proc findPosition(place:int, element:int, numBuckets:int) {
    var position: int;
    position = ((element>>place) & (numBuckets-1));
    return position;
  }

  // To find maximum element in an array
  proc findMaxElement(array:[] int) {
    return max reduce array;
  }

  // To find the place value from which msb sorting starts
  proc findPlace(array:[] int) {
    var place: int;
    var lz: int;
    var maxEl: int;
    var numBits: int;

    maxEl = findMaxElement(array);    
    lz = clz(maxEl);
    numBits = 64 - lz ;
    place = ((numBits)>> 3)<<3;

    return place;
  } 
  
 }

 proc main() {
  
   //var array:[1..10000] int; 
   //fillRandom(array); 
   var array = [6096238063829422255, 17674027062134727, 6497198827793445219, 5051165750459351927, 2546548899626235475, 557962893300217010, 4424852120924984885, 549513434952261278, 2907168857008732820, 695622130240995360, 4259807265639208226, 2625167999958137086];
   
   for i in array.domain {
     array[i] = abs(array[i]);
   }  
   var place = MSBmodule.findPlace(array);

   MSBmodule.radixSortMSB(array,place,1,array.size);
   writeln(array);
   //writeln("sorted array: ",isSorted(array));

  }
