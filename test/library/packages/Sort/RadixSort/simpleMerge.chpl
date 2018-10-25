/* 
  Given an array, and indices demarcating the beginning and end of two sorted subarrays, this procedure merges the two, and copies the result back into the original array.
*/ 
 proc merge(array:[] int, l: int, m: int, r:int) { 
     
   var x: int;
   var y: int;  
   const n1 = m - l + 1; 
   const n2 =  r - m; 
  
   var L:[1..n1] int;  
   var R:[1..n2] int;

   for x in 1..n1 { 
     L[x] = array[l + x - 1]; 
   }
   
   for y in 1..n2 {
     R[y] = array[m + y]; 
   }

   var i = 1; 
   var j = 1;  
   var k = l; 

  while i <= n1 && j <= n2 { 
    if L[i] <= R[j] { 
      array[k] = L[i]; 
      i+=1; 
    } 
    else { 
      array[k] = R[j]; 
      j+=1; 
    } 
    k+=1; 
  } 
  
  while i <= n1 { 
    array[k] = L[i]; 
    i+=1; 
    k+=1; 
  } 
  
  while j <= n2 { 
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
