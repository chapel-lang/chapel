_extern def c_realStr(buf: string, fmt: string, x:real): string;
_extern def c_intStr (buf: string, fmt: string, x:int ): string;

def realStr(fmt: string, x: real) {
    var buf: string = "12345678901234567890123456789012";
    return c_realStr(buf, fmt, x);
}

def intStr(fmt: string, x: int) {
    var buf: string = "12345678901234567890123456789012";
    return c_intStr(buf, fmt, x);
}
