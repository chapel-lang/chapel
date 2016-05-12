//merges two subarrays
//First subarray is arr[l..m]
//Second subarray is arr[m+1..r]
proc merge(arr, left:int, mid:int, right:int){
	var i, j, k: int = 0;
	var left_length: int = mid - left + 1;
	var right_length: int= right - mid;
	
	/* creating temp arrays */
	var left_arr: [1..left_length] int;	
	var right_arr: [1..right_length] int;	
			
	/* Copy data to temp arrays L[] and R[] */
	for i in (1..left_length){
		//writeln("j inside = ", j);		
		left_arr(i) = arr(left -1 + i);
	}
	
	for j in (1..right_length){
		//writeln("j inside = ", j);		
		right_arr(j) = arr(mid + j);
	}

	i = 1;
	j = 1;
	k = left;
	while (i <= left_length && j <= right_length)
    	{
		//writeln("i = ", i, "j = ", j, "k = ", k);		
		if (left_arr(i) <= right_arr(j))
		{
		    arr(k) = left_arr(i);
		    i+=1;
		}
		else
		{
		    arr(k) = right_arr(j);
		    j+=1;
		}
		k+=1;
    	}
	
	/* Copy the remaining elements of left_arr[], if there	are any */
	while (i <= left_length)
	{
		arr(k) = left_arr(i);
		i+=1;
		k+=1;
	}

	/* Copy the remaining elements of right_arr[], if there	are any */
	while (j <= right_length)
	{
		arr(k) = right_arr(j);
		j+=1;
		k+=1;
	}
}

//parallel mergesort algorithm
//heuristics -> When the array size becomes small such that creating a task takes more time than compute time for sorting it, then dont spawn task, rather do it sequentially.
//Heuristic Used(https://antimatroid.wordpress.com/2012/12/01/parallel-merge-sort-in-java/): Threshold set at 256 integer elements, if size < 256 (1024 bytes) elements --> sequential, else parallel.
proc mergesort(arr, left, right){
	if (left < right){

		var mid: int = left + (right-left)/2;
		var size: int = right - left + 1;
		
		if(size < 256){
			//sequential
			mergesort(arr, left, mid);
			mergesort(arr, mid+1, right);		
		}
		else{
			//parallel
			cobegin{
				mergesort(arr, left, mid);
				mergesort(arr, mid+1, right);
			}
		}		
		
		
		merge(arr, left, mid, right);
	}
}

var arr: [1..300] int = (8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19);

writeln("Input Array:");

writeln(arr);

mergesort(arr, 1, 300);

writeln("Sorted Array:");

writeln(arr);
