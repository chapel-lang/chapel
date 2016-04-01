module MPI {
  /* 
   Currently unsupported in the MPI 1.1 standard 
      MPI_Op_create
      MPI_Op_free
      MPI_Errhandler_create
      MPI_Errhandler_free
      MPI_Keyval_create
      MPI_Keyval_free
   */

  use SysCTypes;
  require "mpi.h";

  /******************************
    Defined Constants and Datatypes
   ******************************/

  /* MPI types.
     We define these as opaque types.
     */
  extern type MPI_Aint : opaque;
  extern type MPI_Status : opaque;
  extern type MPI_Group : opaque;
  extern type MPI_Comm : opaque;
  extern type MPI_Datatype : opaque;
  extern type MPI_Request : opaque;
  extern type MPI_Op : opaque;

  // TODO : Not explicily found in the spec
  extern type MPI_Errhandler : opaque;

  /* Return codes. 
     Define these as c_int
     */
  extern const MPI_SUCCESS : c_int;
  extern const MPI_ERR_BUFFER : c_int;
  extern const MPI_ERR_COUNT : c_int;
  extern const MPI_ERR_TYPE : c_int;
  extern const MPI_ERR_TAG : c_int;
  extern const MPI_ERR_COMM : c_int;
  extern const MPI_ERR_RANK : c_int;
  extern const MPI_ERR_REQUEST : c_int;
  extern const MPI_ERR_ROOT : c_int;
  extern const MPI_ERR_GROUP : c_int;
  extern const MPI_ERR_OP : c_int;
  extern const MPI_ERR_TOPOLOGY : c_int;
  extern const MPI_ERR_DIMS : c_int;
  extern const MPI_ERR_ARG : c_int;
  extern const MPI_ERR_UNKNOWN : c_int;
  extern const MPI_ERR_TRUNCATE : c_int;
  extern const MPI_ERR_OTHER : c_int;
  extern const MPI_ERR_INTERN : c_int;
  extern const MPI_PENDING : c_int;
  extern const MPI_ERR_IN_STATUS : c_int;
  extern const MPI_ERR_LASTCODE : c_int;

  /* Assorted constants. 
  TODO : These are defined as opaque, but probably 
  could be more specific.
  */
  extern const MPI_BOTTOM : opaque;
  extern const MPI_PROC_NULL : opaque;
  extern const MPI_ANY_SOURCE : opaque;
  extern const MPI_ANY_TAG : opaque;
  extern const MPI_UNDEFINED : opaque;
  extern const MPI_BSEND_OVERHEAD : opaque;
  extern const MPI_KEYVAL_INVALID : opaque;

  /* Error handling specifiers */
  extern const MPI_ERRORS_ARE_FATAL : MPI_Errhandler;
  extern const MPI_ERRORS_RETURN : MPI_Errhandler;

  /* Maximum sizes for strings. 
  TODO: Are these correctly defined?
  */
  extern const MPI_MAX_PROCESSOR_NAME : c_int;
  extern const MPI_MAX_ERROR_STRING : c_int;

  /* Elementary datatypes */
  extern const MPI_CHAR : MPI_Datatype;
  extern const MPI_SHORT : MPI_Datatype;
  extern const MPI_INT   : MPI_Datatype;
  extern const MPI_LONG  : MPI_Datatype;
  extern const MPI_UNSIGNED_CHAR : MPI_Datatype;
  extern const MPI_UNSIGNED_SHORT : MPI_Datatype;
  extern const MPI_UNSIGNED : MPI_Datatype;
  extern const MPI_UNSIGNED_LONG : MPI_Datatype;
  extern const MPI_FLOAT  : MPI_Datatype;
  extern const MPI_DOUBLE  : MPI_Datatype;
  extern const MPI_LONG_DOUBLE : MPI_Datatype;
  extern const MPI_BYTE  : MPI_Datatype;
  extern const MPI_PACKED : MPI_Datatype;

  /* Datatypes for reductions */
  extern const MPI_FLOAT_INT : MPI_Datatype;
  extern const MPI_DOUBLE_INT : MPI_Datatype;
  extern const MPI_LONG_INT : MPI_Datatype;
  extern const MPI_2INT : MPI_Datatype;
  extern const MPI_SHORT_INT : MPI_Datatype;
  extern const MPI_LONG_DOUBLE_INT : MPI_Datatype;

  /* Optional datatypes */
  extern const MPI_LONG_LONG_INT : MPI_Datatype;

  /* Special datatypes for constructing derived datatypes */
  extern const MPI_UB : MPI_Datatype;
  extern const MPI_LB : MPI_Datatype;

  /* Reserved communicators */
  extern const MPI_COMM_WORLD : MPI_Comm;
  extern const MPI_COMM_SELF : MPI_Comm;

  /* Communicator/ Group comparisons */
  // These all appear to be C integers in MPI code
  extern const MPI_IDENT : c_int;
  extern const MPI_CONGRUENT : c_int;
  extern const MPI_SIMILAR : c_int;
  extern const MPI_UNEQUAL : c_int;

  /* Environmental inquiry keys */ 
  // All of the above appear to be C enums or defines....
  extern const MPI_TAG_UB : c_int; 
  extern const MPI_IO : c_int; 
  extern const MPI_HOST : c_int; 
  extern const MPI_WTIME_IS_GLOBAL : c_int; 

  /* Collective operations */
  extern const MPI_MAX : MPI_Op;
  extern const MPI_MIN : MPI_Op;
  extern const MPI_SUM : MPI_Op;
  extern const MPI_PROD : MPI_Op;
  extern const MPI_MAXLOC : MPI_Op;
  extern const MPI_MINLOC : MPI_Op;
  extern const MPI_BAND : MPI_Op;
  extern const MPI_BOR : MPI_Op;
  extern const MPI_BXOR : MPI_Op;
  extern const MPI_LAND : MPI_Op;
  extern const MPI_LOR : MPI_Op;
  extern const MPI_LXOR : MPI_Op;

  /********************************************
   FUNCTION DECLARATIONS GO BELOW
   *******************************************/


}
