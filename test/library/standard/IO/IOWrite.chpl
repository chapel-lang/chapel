use IO;

proc PrintIOWrite(const input){
 assert(write(input));
 assert(writeln(input));

}
PrintIOWrite(12);
PrintIOWrite(12.5);
PrintIOWrite("Hello");
