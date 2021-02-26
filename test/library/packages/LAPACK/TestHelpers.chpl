module TestHelpers {
  public use LAPACK;
  param default_epsilon: real = 10.0e-14;  

  class LAPACK_Matrix {
    type data_type;
    var row_major: bool;
    var rows: int, columns: int;
    var data_domain: domain(2);
    var matrix_domain: domain(2);
    var data: [data_domain] data_type;
    var epsilon: real = default_epsilon;
    
    proc init( type data_type, rows: int, columns: int, matrix_order: lapack_memory_order, error: real = default_epsilon  ){
    
      this.data_type = data_type;
      this.row_major = isLAPACKRowMajor( matrix_order );
      this.rows = rows;
      this.columns = columns;

      if this.row_major then
        this.data_domain = {1..#rows,1..#columns};
      else
        this.data_domain = {1..#columns,1..#rows};
      this.matrix_domain = {1..#rows,1..#columns};
      this.epsilon = error;
    }
    
    proc init( type data_type, rows: int, columns: int, matrix_order: lapack_memory_order, 
                        error: real = default_epsilon, input_array: [?D] data_type, 
                        input_array_order: lapack_memory_order = matrix_order )
    {
      this.data_type = data_type;
      this.row_major = isLAPACKRowMajor( matrix_order );
      this.rows = rows;
      this.columns = columns;

      if this.row_major then
        this.data_domain = {1..#rows,1..#columns};
      else
        this.data_domain = {1..#columns,1..#rows};
      this.matrix_domain = {1..#rows,1..#columns};
      this.epsilon = error;
      
      this.complete();
      this.populateFromArray( input_array, input_array_order );
      
    }
    
    proc init( type data_type, rows: int, columns: int, row_order: bool, error: real = default_epsilon ){
      this.data_type = data_type;
      this.row_major = row_order;
      this.rows = rows;
      this.columns = columns;
      if this.row_major then
        this.data_domain = {1..#rows,1..#columns};
      else
        this.data_domain = {1..#columns,1..#rows};
      this.matrix_domain = {1..#rows,1..#columns};
      this.epsilon = error;
    }
    
    proc init( type data_type, rows: int, columns: int, row_order: bool, 
                        error: real = default_epsilon, input_array: [?D] data_type, 
                        input_row_order = row_order )
    {
      this.data_type = data_type;
      this.row_major = row_order;
      this.rows = rows;
      this.columns = columns;
      if this.row_major then
        this.data_domain = {1..#rows,1..#columns};
      else
        this.data_domain = {1..#columns,1..#rows};
      this.matrix_domain = {1..#rows,1..#columns};
      this.epsilon = error;
      
      this.complete();
      this.populateFromArray( input_array, input_row_order );
      
    }
    
    proc init( matrix: borrowed LAPACK_Matrix, type data_type = matrix.data_type ){
      this.data_type = data_type;
      this.row_major = matrix.row_major;
      this.rows = matrix.rows;
      this.columns = matrix.columns;
      this.data_domain = matrix.data_domain;
      this.matrix_domain = matrix.matrix_domain;

      this.complete();

      for idx in data_domain do this.data[idx] = matrix.data[idx];
      this.epsilon = matrix.epsilon;
    }
    
    proc populateFromArray( array: [?d] data_type, matrix_order: lapack_memory_order ): void {
      this.populateFromArray( array, isLAPACKRowMajor( matrix_order ) );
    }
    
    proc populateFromArray( array: [?d] data_type, arrayIsRowMajor: bool): void {
      var dimensions = d.dims();
      assert( dimensions.size <= 2 );
      
      if dimensions.size == 1 {
        var arrayRange: range = dimensions[0];
        var shift = arrayRange.low - 1;
        //writeln( "(i, j, idx )" );
        for (i,j) in matrix_domain {
          var idx = if arrayIsRowMajor then (i-1)*this.columns + j + shift 
                                       else (j-1)*this.rows    + i + shift;
          
          //writeln( (i, j, idx ) );
          this[i,j] = array[ idx ];

        }
      } else {
        var array_leading = if arrayIsRowMajor then 0 else 1;
        var array_following = 1-array_leading;
        var trans = ( d.dim(array_leading).low - this.matrix_domain.dim(0).low,
                      d.dim(array_following).low - this.matrix_domain.dim(1).low );
                      
        for (i,j) in this.matrix_domain {
          this[i,j] = if arrayIsRowMajor then array[i+trans[0],j+trans[1]] 
                                         else array[j+trans[0],i+trans[1]] ;
        }
      }
    }
    
    proc populateFromArray( array: [?d] data_type ): void{
      this.populateFromArray( array, this.isRowMajor );
    }
    
    proc this(i: int, j: int) ref : data_type{
      if row_major then
        return data[i,j];
      else
        return data[j,i];
    }
    
    proc this( idx: 2*int ) ref : data_type {
      return this( idx[0], idx[1] );
    }
    
    proc rowRange : range {
      return matrix_domain.dim(0);
    }
    
    proc columnRange : range {
      return matrix_domain.dim(1);
    }
    
    proc toString(): string {
      var retstring: string = "";
      for i in this.rowRange{
        retstring += if i == 1 then ( "[ " )
                               else ( "  " );
                              
        for j in this.columnRange {
          retstring += ( this[i,j]:string + ", " );
        }
        
        retstring += if i == this.rows then ( "]" )
                                       else ( "\n" );
      }
      return retstring;
    } 
    
    proc storage_order: lapack_memory_order{
      return if row_major then lapack_memory_order.row_major else lapack_memory_order.column_major;
    }
    
    proc isRowMajor: bool {
      return row_major;
    }
    
    proc leadingDimension: int {
      return this.data_domain.dim(1).size;
    }
    
    proc order: int {
      assert( this.rows == this.columns );
      return this.rows;
    }
    
  }
  
  proc isLAPACKRowMajor( matrix_order: lapack_memory_order ): bool {
    return (matrix_order == lapack_memory_order.row_major);
  }
  
  // Not using type queries to work around issue #13721
  proc *( A: borrowed LAPACK_Matrix, B: borrowed LAPACK_Matrix ): owned LAPACK_Matrix(A.data_type) {
    if A.data_type != B.data_type then
      compilerError("data_type mismatch in *");

    assert( A.columns == B.rows );
    var row_ordered = if ( A.isRowMajor &&  B.isRowMajor)
                      || (!A.isRowMajor && !B.isRowMajor) 
                      then A.isRowMajor
                    else
                      true;
    
    var retmatrix = new owned LAPACK_Matrix( A.data_type, A.rows, B.columns, row_ordered, error = min( A.epsilon, B.epsilon ) );
    
    for i in A.rowRange do
      for j in B.columnRange do
        for k in A.columnRange do // could also be B.colRange
          retmatrix[i,j] += A[i,k] * B[k, j];
 
    return retmatrix;
  }
  
  proc ==( A: borrowed LAPACK_Matrix, B: borrowed LAPACK_Matrix ): bool {
    if A.data_type != B.data_type then
      compilerError("data_type mismatch in ==");

    //if A == nil then halt( "A is nil" );
    //if B == nil then halt( "B is nil" );
    if !( A.rows == B.rows && A.columns == B.columns ) then
      return false;
    
    var epsilon = min( A.epsilon, B.epsilon );
    
    var equal = true;
    for idx in A.matrix_domain {
      if !epsilonEquals( A[idx], B[idx], epsilon ){
        equal = false;
        break;
      }
    }  
    
    return equal;
  }
  
  inline proc epsilonEquals( a: ?T, b: T, epsilon: real = default_epsilon ): bool 
    where isNumericType( T ){
    return ( abs( a - b ): real) < epsilon;
  }
  
  inline proc epsilonEquals( A: [] ?T, B: [] T, epsilon: real = default_epsilon ): bool 
    where isNumericType( T ){
    if A.domain != B.domain then
      return false;
      
    var equal = true;
    for idx in A.domain {
      if !epsilonEquals( A[idx], B[idx], epsilon ){
        equal = false;
        break;
      }
    }
    
    
    return equal;
  }
  
  proc copy_from_to( from: [?D] ?t, to: [D] t ): void {
    for x in D do
      to[x] = from[x];
  }
  
}
