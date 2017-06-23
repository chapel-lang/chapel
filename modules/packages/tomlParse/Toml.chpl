/* 
TOML library  written in chapel and used in the chapel package manager
*/


/*
parseToml takes in a channel to a .toml file as an argument and outputs an 
associative array based on the contents of the .toml file. This array
can then be passed to a writeToml method to write to a .toml file.
*/ 
proc parseToml(input: channel) {}


/*
parseTomlStr takes in a string in toml format to be parse into an associative
array. The array can be printed using the writeToml procedure. 
*/
proc parseTomlStr(input: string) {}


/*
Takes in a channel to an output file provided as an argument. The format
for writing into the file is TOML.  
*/
proc writeToml(output: channel) {}



class Parser {}