proc partition(arr, left: int, right: int) : int{
	var x : int = arr(right);
	var i : int = (left - 1);
 
	for j in (left..right-1){
		if (arr(j) <= x)
        	{
            		i+=1;
			//swap
			arr(i) <=> arr(j);
		}
    	}
	//swap last
	arr(i+1) <=> arr(right);
	
    	return (i + 1);
}

proc quickselect(arr, left, right, k) : int{
    	// If k is more than number of elements in array
    	if ((left + k) >= left && k <= right - left + 1){
		
		
		var pivot_index: int = partition(arr, left, right);
	

		// If position is same as k	
		if(pivot_index - left + 1 == k){
			return arr(pivot_index);
		}		
		//If kth ele exists left side of arr	
		if(pivot_index - left + 1 > k){
			return quickselect(arr, left, pivot_index-1, k);	
		}		
		//Else kth ele exists right side of arr
		return quickselect(arr, pivot_index + 1, right, k + left - pivot_index - 1);
	}
	else{
		writeln("Sorry, k should be within the element range");
		return -1;
	}
}

var arr: [1..100] int = (8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19);

config var k: int = 1;

writeln("Input Array:");

writeln(arr);

var ele : int = quickselect(arr, 1, 100, k);

writeln(k, "th ele = ", ele);
