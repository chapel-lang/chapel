
module flagsortMSB {
  use Sort;
  use Time;
  use BitOps;
  use Random;
  config param bucketBits=8;
  param numBuckets = (1 << bucketBits);
  param ElementsToBufferDuringShuffle = 5;
  config const tryUnroll = false;

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
  
   /* MSB helper function */
   proc radixSortMSB(array:[?dom] int) {

     var place = findPlace(array);
     __radixSortMSB(array,place,dom.low,dom.high);

   }

   private proc __radixSortMSB(array:[] int, place: int, startIndex: int, endIndex: int) {

     // base case, exit condition
     if (place<0 || endIndex <= startIndex ) {
       return;
     }

     if(endIndex - startIndex < 100) {
       sort(array[startIndex..endIndex]);
     }

    var counts:[0..numBuckets] int;
    var curOffsets:[0..numBuckets] int;

    for i in startIndex..endIndex {
      var bin = findPosition(place,array[i], numBuckets);
      counts[bin] += 1;
    }

    curOffsets = + scan counts;
    for (off,end) in zip(counts,curOffsets) {
      off = startIndex + end - off;
      end = startIndex + end;
    }
 
    var curbin = 0;

    /* Shuffle/re-distribute the elements according to the key part currently under consideration */
    
    while true {

      while curbin <= numBuckets && counts[curbin] == curOffsets[curbin] {
        curbin += 1;
      }
      if curbin > numBuckets {
        break;
      }

      param max_buf = ElementsToBufferDuringShuffle;
      var buf: max_buf*array.eltType;
      var used_buf = 0;
      var end = curOffsets[curbin];
      var endfast = max(counts[curbin], curOffsets[curbin]-2*max_buf);
      var bufstart = max(counts[curbin], curOffsets[curbin]-max_buf);

     
      var i = bufstart;
    
      while i < end {
        buf[used_buf+1] = array[i];
        used_buf += 1;
        i += 1;
      }
      
      if tryUnroll {
        while counts[curbin] < endfast {
          for param j in 1..max_buf {
            var bin = findPosition(place,buf[j],numBuckets);
            array[counts[bin]] <=> buf[j];
            counts[bin] += 1;
          }
        }
      }

      while counts[curbin] < end {
        var j = 1;
        while used_buf > 0 && j <= used_buf {
          var bin = findPosition(place,buf[j],numBuckets);
          if bin == curbin && counts[curbin] >= bufstart {
              array[counts[bin]] = buf[j];
              buf[j] = buf[used_buf];
              used_buf -= 1;
          }
          else {
            array[counts[bin]] <=> buf[j];
          }
          counts[bin] += 1;
          j += 1;
        }
      }
    }

    /* reset offsets as before swapping: for demarcating buckets using offsets array. */
    counts[0] = startIndex;
    for i in 1..numBuckets {
      counts[i] = curOffsets[i-1];
    }
 
    for bin in 0..numBuckets {
      const binStart = counts[bin];
      const binEnd = if bin+1<=numBuckets then counts[bin+1]-1 else endIndex;
      const num = 1 + binEnd - binStart;
      if num > 1 {
         __radixSortMSB(array,place-bucketBits,binStart, binEnd);
      }       
    }	
  }
}

