//Functions for uppper triangular matrix and lower triangular matrix

proc upper(arr,r, c): (bool) {
	for i in 1..r{
		for j in 1..c{
			if(i<j&&arr[i][j]!=0)
			{
				return false;
			}
		}
	}
	return true;
}
proc lower(arr,r,c): (bool) {
	for i in 1..r{
		for j in 1..c{
			if(i>j&&arr[i][j]!=0)
			{
				return false;
			}
		}
	}
	return true;
}