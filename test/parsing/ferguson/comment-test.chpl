/*
 test a:
 A normal comment
 */

writeln("a");

/*
 test b:
 A /* nested */ comment
 */

writeln("b");

/**
 test c:
 A block comment, 
   */ 
   SHOULD_BE_COMMENTED_OUT1; 
   /* are ignored.
 **/

writeln("c");

/***
 test d:
 A block comment, 
   */ 
   SHOULD_BE_COMMENTED_OUT2; 
   /* are ignored.
   
   **/ 
   SHOULD_BE_COMMENTED_OUT3; 
   /** are ignored.

 ***/

writeln("d");

/****
 test e:
 A block comment, 
   */ 
   SHOULD_BE_COMMENTED_OUT4; 
   /* are ignored.
    
   **/ 
   SHOULD_BE_COMMENTED_OUT5; 
   /** are ignored.

   ***/ 
   SHOULD_BE_COMMENTED_OUT6; 
   /*** are ignored.

 ****/


writeln("e");

/**
 test f:
 this little guy normally counts as start-comment
   /*/

 **/

writeln("f");

/**
 test g:
 nested ** comments
 
 /** nested **/
 /*** nested ***/
 /**** nested ****/
 /***** nested *****/

 **/

writeln("g");

/***
 test h:
 nested *** comments
 
 /*** nested ***/
 /**** nested ****/
 /***** nested *****/

 ***/

writeln("h");

/****
 test i:
 nested **** comments
 
 /**** nested ****/
 /***** nested *****/

 ****/

writeln("i");

