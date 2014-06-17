extern {
 // } "}" '}' '\ "\ e
 /* // } "}" '}' '\ "\ e 

    note that c does not have nested comments
    so this ends the comment
 */
 static const char az = '}';
 static const char * bz = "}\\}";
 static int foo(int x) {
   {
     return x;
   }
 }

}

