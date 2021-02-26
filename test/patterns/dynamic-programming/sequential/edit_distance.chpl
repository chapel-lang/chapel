proc min3(x:int,y:int,z:int):int{
   return min(min(x, y), z);
}

proc editDist(str1:string, str2:string, len1:int, len2:int):int{
    	// If first string is empty, the only option is to
   	// insert all characters of second string into first
    	if(len1 == 0){
		return len1;
 	}
	// If second string is empty, the only option is to
    	// remove all characters of first string
    	if (len2 == 0){
		return len1;
 	}
	
    	// If last characters of two strings are same, nothing
    	// much to do. Ignore last characters and get count for
    	// remaining strings.
    	if (str1[len1-1] == str2[len2-1]){
        	return editDist(str1, str2, len1-1, len1-1);
 	}
    	// If last characters are not same, consider all three
    	// operations on last character of first string, recursively
    	// compute minimum cost for all three operations and take
    	// minimum of three values.
    	return 1 + min3 ( editDist(str1,  str2, len1, len2-1), editDist(str1,  str2, len1-1, len2), editDist(str1,  str2, len1-1, len2-1));
}

var str1:string = "sunday";
var str2:string = "saturday";

writeln("The edit distance between ", str1, " and ", str2, " is: ", editDist(str1, str2, str1.buffLen, str2.buffLen)); 
