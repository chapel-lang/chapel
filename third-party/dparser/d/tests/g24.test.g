S: number*;
number ::= integer | longinteger | floatnumber | imagnumber;
integer ::= decimalinteger | octinteger | hexinteger;
decimalinteger ::= nonzerodigit digit* | '0';
octinteger ::= '0' octdigit+;
hexinteger ::= '0' ('x' | 'X') hexdigit+;
floatnumber ::= pointfloat | exponentfloat;
pointfloat ::= intpart? fraction | intpart '.';
exponentfloat ::= (intpart | pointfloat) exponent;
intpart ::= digit+;
fraction ::= "." digit+;
exponent ::= ("e" | "E") ("+" | "-")? digit+;
imagnumber ::= (floatnumber | intpart) ("j" | "J");
longinteger ::= integer ("l" | "L");
nonzerodigit ::= "[1-9]";
digit ::= "[0-9]";
octdigit ::= "[0-7]";
hexdigit ::= digit | "[a-fA-F]";

