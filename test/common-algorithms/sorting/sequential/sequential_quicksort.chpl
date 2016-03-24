//var arr: [1..10] int = (8, 14, -8, -9, 5, -9, -3, 0, 17, 19);

proc partition(arr, left: int, right: int) : int{
	var x : int = arr(right);
	var i : int = (left - 1);
 
	for j in (left..right-1){
		if (arr(j) <= x)
        	{
            		i+=1;
            		var temp : int = arr(j);
			arr(j) = arr(i);
			arr(i) = temp;
        	}
    	}
    	var temp : int = arr(i+1);
	arr(i+1) = arr(right);
	arr(right) = temp;
    	return (i + 1);
}

proc quicksort(arr, left, right){
	if (left < right){
		
	
	
	
		var pivot_index: int = partition(arr, left, right);
		quicksort(arr, left, pivot_index-1);
		quicksort(arr, pivot_index+1, right);
		
		
		/*
		for i in (left, pivot_index-1){
			write(arr(i), " ");
		}
		writeln("");
		for i in (pivot_index, right){
                        write(arr(i), " ");
                }
                writeln("");
		*/
	}
}

var arr: [1..100] int = (8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19, 8, 14, -8, -9, 5, -9, -3, 0, 17, 19);

writeln("Input Array:");
for i in (1..100){
        write(arr(i), " ");
}
writeln("");

quicksort(arr,1, 100);

writeln("Sorted Array:");
for i in (1..100){
	write(arr(i), " ");
}
writeln("");
