use MultiDomain_def;



proc main {
  
  var mD = new MultiDomain(2,false);
  mD.add( {1..12, 1..12} );
  mD.subtract( {3..5, 4..7} );
  mD.subtract( {4..9, 6..8} );
  mD.prettyPrint();
  writeln();



  
  var mA = new MultiArray(2,false,int);
  mA.allocate( mD );
  
  var i = 0;
  for array in mA {
    i+=1;
    array = i;
  }
  
  var A_full: [mD.root.Domain] int;
  for array in mA do
    A_full(array.domain) = array;
    
  writeln(A_full);
  
  
}




//|\""""""""""""""""""""""""""|\
//| >    class: MultiArray    | >
//|/__________________________|/

//-----------------------------------------------------
// A collection of arrays, designed to store values on
// all domains in a MultiDomain.  Enforcement of this
// is fairly weak.
//-----------------------------------------------------

class MultiArray
{
  
  param rank: int;
  param stridable: bool;
  type  eltType;
  
  var array_wrappers: List( ArrayWrapper );



  //---- Subclass that wraps an array and its domain together ----
  //-----------------------------------------------------------------
  // This prevents an array from being typed by its domain, allowing
  // them to be collected in a single data structure.
  //-----------------------------------------------------------------
  
  class ArrayWrapper
  {
    var Domain: domain(outer.rank, stridable=outer.stridable);
    var array: [Domain] outer.eltType;
  }


  proc initialize () { array_wrappers = new List(ArrayWrapper); }


  proc ~MultiArray ()
  {
    for wrapper in array_wrappers do delete wrapper;
    delete array_wrappers;
  }
  


  iter these () ref
  {
    for wrapper in array_wrappers do 
      yield wrapper.array;
  }




  //|\'''''''''''''''''''''''''|\
  //| >    method: allocate    | >
  //|/.........................|/
  
  //----------------------------------------------------------
  // Allocates storage in the MultiArray corresponding to the
  // domains in a MultiDomain.
  //----------------------------------------------------------

  // proc allocate ( mD: MultiDomain(rank,stridable) ) 
  // {
  //   for D in mD do
  //     array_wrappers.add( new ArrayWrapper(D) ); 
  // }
  
  proc allocate ( mD: MultiDomain(rank,stridable) )
  {
    for D in mD do array_wrappers.add( new ArrayWrapper(D) ); 
  }
  // /|'''''''''''''''''''''''''/|
  //< |    method: allocate    < |
  // \|.........................\|
  
  
  
}
// /|""""""""""""""""""""""""""/|
//< |    class: MultiArray    < |
// \|__________________________\|







// //|\"""""""""""""""""""""""""""|\
// //| >    ArrayWrapper class    | >
// //|/___________________________|/
// 
// //----------------------------------------------------------------
// // This class is designed to enable an "array of arrays". It
// // wraps an array and its domain into a single object, preventing
// // the array from being typed by its domain.
// //----------------------------------------------------------------
// 
// class ArrayWrapper {
//   param rank: int;
//   param stridable: bool;
//   type  eltType;
//   
//   var Domain: domain(rank=rank, stridable=stridable);
//   var array: [Domain] eltType;
//   
//   
//   //|\''''''''''''''|\
//   //| >    clear    | >
//   //|/..............|/
//   proc clear () {
//     Domain.clear();
//   }
//   // /|''''''''''''''/|
//   //< |    clear    < |
//   // \|..............\|
//   
// }
// // /|"""""""""""""""""""""""""""/|
// //< |    ArrayWrapper class    < |
// // \|___________________________\|
