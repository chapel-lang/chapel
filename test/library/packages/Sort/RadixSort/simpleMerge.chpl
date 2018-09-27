proc merge(array:[] int, l: int, m: int, r:int){ 
    
    var i: int;
    var j: int; 
    var k: int; 
    var n1 = m - l + 1; 
    var n2 =  r - m; 
  
    var L:[1..n1] int;  
    var R:[1..n2] int;

    for i in 1..n1 { 
        L[i] = array[l + i - 1]; 
    }
    for j in 1..n2 {
        R[j] = array[m + j]; 
    }

    i = 1; 
    j = 1;  
    k = l; 

    while (i <= n1 && j <= n2) { 
        if (L[i] <= R[j]) { 
            array[k] = L[i]; 
            i+=1; 
        } 
        else { 
            array[k] = R[j]; 
            j+=1; 
        } 
        k+=1; 
    } 
  
    while (i <= n1) { 
        array[k] = L[i]; 
        i+=1; 
        k+=1; 
    } 
  
    while (j <= n2) { 
        array[k] = R[j]; 
        j+=1; 
        k+=1; 
    } 
}

proc main() {

var array = [1,3,5,7,9,4,6,8,10,12];
merge(array,1,5,10);
writeln(array);
} 
