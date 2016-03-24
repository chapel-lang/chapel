
//merges two subarrays
//First subarray is arr[l..m]
//Second subarray is arr[m+1..r]

proc merge(arr, left:int, mid:int, right:int){
	var i: int = 0, j: int = 0, k: int = 0;
	var left_length: int = mid - left + 1;
	var right_length: int= right - mid;
	
	/* create temp arrays */
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
//can improve performance by adding heuristics -> When the array size becomes small such that creaing a task takes more time than compute time for sorting it, then dont spawn task, rather do it sequentially.
proc mergesort(arr, left, right){
	if (left < right){
		var mid: int = left + (right-left)/2;
		
		cobegin{
			mergesort(arr, left, mid);
			mergesort(arr, mid+1, right);
		}
		
		/*
		for i in (left, mid){
			write(arr(i), " ");
		}
		writeln("");
		for i in (mid+1, right){
                        write(arr(i), " ");
                }
                writeln("");
		*/
		
		merge(arr, left, mid, right);
	}
}

var arr: [1..100] int = (8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19);

writeln("Input Array:");
for i in (1..100){
        write(arr(i), " ");
}
writeln("");

mergesort(arr,1, 100);

writeln("Sorted Array:");
for i in (1..100){
	write(arr(i), " ");
}
writeln("");
