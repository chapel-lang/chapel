
// too many semicolons
{
  var arr1 = [1, 2;; 3, 4];

  var arr2 = [1,2,3 ;; 4,5,6 ;; 7,8,9]; 

  var arr3 = [1, 2; 
              3, 4;
              ;;
              5, 6;
              7, 8;];

  var arr4 = [1, 2;; 
              3, 4;;
              ;;
              5, 6;;
              7, 8;;];

  var arr5 = [1, 2; 
              3, 4;
              ;;
              5, 6;
              7, 8;
              ;;
              9, 10;
              11, 12];
}

// missing elements
{
  var arr4 = [1, 2;
              3, /*4*/];

  var arr5 = [1, /*2*/;
              3, 4];

  var arr6 = [1, 2, 3;
              4, 5, 6;;
              7, 8, /*9*/;
              10, 11, 12;];
}

// too many trailing semicolons
{
  var arrCol = [1;;];

  var arrColTrailingComma = [1,;;];

  var arr3d = [1, 2; 3, 4;; 5, 6; 7, 8;;];
  
  var arr3dTrailingComma = [1, 2; 3, 4;; 5, 6; 7, 8,;;];
}
