_extern def sprintf(buf: string, fmt: string, x:real): string;
_extern def sprintf(buf: string, fmt: string, x:int ): string;

def realStr(fmt: string, x: real) {
    var buf: string = "12345678901234567890123456789012";
    sprintf(buf, fmt, x);
    return buf;
}

def intStr(fmt: string, x: int) {
    var buf: string = "12345678901234567890123456789012";
    sprintf(buf, fmt, x);
    return buf;
}
