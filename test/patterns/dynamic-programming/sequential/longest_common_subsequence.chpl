proc lcs(string1:string, string2:string, len1:int, len2:int): int{
	if (len1 == 0 || len2 == 0){
		return 0;
	}
   	if (string1(len1-1) == string2(len2-1)){
     		return 1 + lcs(string1, string2, len1-1, len2-1);
	}
   	else{
     		return max(lcs(string1, string2, len1, len2-1), lcs(string1, string2, len1-1, len2));
	}
}

var string1: string = "AGGTAB";
var string2: string = "GXTXAYB";

writeln("Length of Longest Common Subsequence is: ", lcs(string1, string2, string1.len, string2.len));
