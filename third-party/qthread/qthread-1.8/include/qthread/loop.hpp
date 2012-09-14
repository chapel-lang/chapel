/* How to make the loops calls -- parameter list madness...

   This file has two purposes:
   1) quick reference for how to set up loop calls
      Find the type of loop you want and hopefully
      the lists of macros will remind you how to
      order all the parameters
   2) If you really like pound defines or are having
      trouble getting a loop call to compile
      (No matching call error says very little...)
      Then just #define the flavor of loop
      Then #define all of the macro values for that flavor
      Then #include "loop.hpp"

      Example:
      #define VOID_LOOP
      #define LOOP_TYPE mt_loop_traits::Future
      #define FUNC_PTR foo
      #define ARG_TYPES Iterator, int
      #define PARAM_LIST NULL, my_int
      #define LOOP_START 0
      #define LOOP_STOP limit
      #include "loop.hpp"


   RET_TYPE: the return type of the function being called
   For functions returning void, the Void versions of loop
   calls can be used and no RET_TYPE need be specified

   RET_PTR: pointer to an array where return values
   will be stored. This array must have elements
   >= the number of loop iterations

   FUNC_PTR: pointer to the function which will
   be called a the new thread in each iteration...
   Yes, all loops make a single function call.
   Many auxiliary functions will be written >_<

   ARG_TYPES: the argument signature of the function
   This is a comma separated list of all the types taken
   by the function as parameters, in the order they appear
   Reference type arguments are not currently supported. 
   Maximum number of arguments is currently 5.

   PARAM_LIST: parameter values to be sent to the looped function
   Iterators should use a NULL value
   Array<T> arguments should send a T*
   Everyone else works as (hopefully) expected ^_^

   CLASS_NAME: Name of the class for member function calls

   OBJ_PTR: object to which all calls will be directed...
   There is no current support for calling to different
   objects on each iteration. Do this by making an
   auxiliary function that takes an object pointer
   and an Iterator and explicitly making the call.

   LOOP_START: start index for iterations
   LOOP_STOP: stop index for iterations
   LOOP_STEP: stepping value for subsequent iterations
              default value is 1
   Read this as:
   for (i = LOOP_START; i < LOOP_STOP; i+= LOOP_STEP)

   Most important thing: Return and Array values
   should be equal or greater in size than
   the number of iterations... there is no
   support for bounds checking at the moment

   LOOP_TYPE: A supported loop type
   Current options are:
   mt_loop_traits::Par - a regular fork and join type loop
   mt_loop_traits::Future - a resource constrained loop, good
                  for recursive calls.
		  What does this mean? "Resource Constrained"?
		  It means the loop will not create new threads
		  freely, but waits for threads to finish
		  before spawning more.
   mt_loop_traits::ParNoJoin,
   mt_loop_traits::FutureNoJoin - 
   There are versions of the same loops that do
   not wait for the iterations to finish before
   moving onto the next instruction...
   Use with Caution!
   For any assurance that an iteration finishes before the whole
   application terminates, provide explicit syncronization
   Don't be like me: Don't send pointers to local variables into
   a threaded loop unless you ensure the calling function
   will not terminate before all loop iterations finish

Note: 
These are squinty eyes: >_<
These are happy eyes: ^_^
*/

#ifndef LOOP_STEP
#define LOOP_STEP 1
#endif

#ifdef LOOP

#define CALL_LOOP() mt_loop_returns<RET_TYPE,ARG_TYPES,LOOP_TYPE> \
  (RET_PTR, FUNC_PTR, PARAM_LIST, LOOP_START, LOOP_STOP, LOOP_STEP)

#undef LOOP

#elif (defined VOID_LOOP)

#define CALL_LOOP() mt_loop<ARG_TYPES,LOOP_TYPE> \
  (FUNC_PTR, PARAM_LIST, LOOP_START, LOOP_STOP, LOOP_STEP)

#undef VOID_LOOP

#elif (defined MEMBER_LOOP)

#define CALL_LOOP() mt_mfun_loop_returns<CLASS_NAME,RET_TYPE,ARG_TYPES,LOOP_TYPE> \
  (OBJ_PTR, RET_PTR, FUNC_PTR, PARAM_LIST, LOOP_START, LOOP_STOP, LOOP_STEP)

#undef MEMBER_LOOP

#elif (defined VOID_MEMBER_LOOP)

#define CALL_LOOP() mt_mfun_loop<CLASS_NAME,ARG_TYPES,LOOP_TYPE>	\
  (OBJ_PTR, FUNC_PTR, PARAM_LIST, LOOP_START, LOOP_STOP, LOOP_STEP)

#undef VOID_MEMBER_LOOP

#endif

CALL_LOOP();

#undef ARG_TYPES
#undef LOOP_TYPE
#undef FUNC_PTR
#undef PARAM_LIST
#undef LOOP_START
#undef LOOP_STOP
#undef LOOP_STEP

#ifdef RET_TYPE
#undef RET_TYPE
#undef RET_PTR
#endif

#ifdef CLASS_NAME
#undef CLASS_NAME
#undef OBJ_PTR
#endif

#undef CALL_LOOP
