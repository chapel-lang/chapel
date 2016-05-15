//Partitions an array into left and right keeping the pivot in between. All elements to the left are less than pivot element and all elements to right of pivot element are greater than the pivot element. Method returns the pivot index. 
proc partition(arr, left: int, right: int) : int{
	var x : int = arr(right);
	var i : int = (left - 1);
 
	for j in (left..right-1){
		if (arr(j) <= x)
        	{
            		i+=1;
            		arr(i) <=> arr(j);
        	}
    	}
    	arr(i+1) <=> arr(right);
    	return (i + 1);
}

//parallel quicksort algorithm
//heuristics -> When the array size becomes small such that creating a task takes more time than compute time for sorting it, then dont spawn task, rather do it sequentially.
//Heuristic Used(https://antimatroid.wordpress.com/2012/12/01/parallel-merge-sort-in-java/): Threshold set at 256 integer elements, if size < 256 (1024 bytes) elements --> sequential, else parallel.
proc quicksort(arr, left, right){
	if (left < right){
		var pivot_index: int = partition(arr, left, right);
		var size: int = left - right + 1;
		if(size < 256){		
			//sequential
			quicksort(arr, left, pivot_index-1);
			quicksort(arr, pivot_index+1, right);
		}
		else{
			//parallel			
			cobegin{
				quicksort(arr, left, pivot_index-1);
				quicksort(arr, pivot_index+1, right);
			}
		}
	}
}

var arr: [1..300] int = (8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19);

writeln("Input Array:");

writeln(arr);

quicksort(arr, 1, 300);

writeln("Sorted Array:");

writeln(arr);
