//Time Complexity: O(mn)
proc count(S, m:int, n:int):int{
	// table[i] will be storing the number of solutions for value i. We need n+1 rows as the table is constructed in bottom up manner using the base case (n = 0)
	var table: [1..n+1] int = 0;
	
	// Base case (If given value is 0)
	table(1) = 1;
 	
	// Pick all coins one by one and update the table[] values after the index greater than or equal to the value of the picked coin
	for i in (1..m){
		for j in (S(i)..n){
			table(j) += table(j-S(i)+1);
		}
	}
	
	return table(n);
}

var arr: [1..3] int = (1, 2, 3);
var m : int = 3;
var n : int = 4;

writeln("The count = ", count(arr, m, n));
