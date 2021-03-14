// Returns the maximum value that can be put in a knapsack of capacity W
proc knapSack(weight:int, wt_list, val_list, n:int):int{
	// Base Case
	if(n == 0 || weight == 0){
		return 0;
 	}
   	// If weight of the nth item is more than Knapsack capacity W, then this item cannot be included in the optimal solution
   	if (wt_list[n] > weight){
   		return knapSack(weight, wt_list, val_list, n-1);
 	}
   	// Return the maximum of two cases: 
   	// (1) nth item included 
   	// (2) not included
   	else{
		return max(val_list[n] + knapSack(weight-wt_list[n], wt_list, val_list, n-1), knapSack(weight, wt_list, val_list, n-1));
	}
}

var val_list:[1..3] int = (60,100,120);
var wt_list:[1..3] int = (10,20,30);

var weight: int = 50;

var n: int = 3;

writeln("Knapsack optimized cost = ", knapSack(weight, wt_list, val_list, n));
