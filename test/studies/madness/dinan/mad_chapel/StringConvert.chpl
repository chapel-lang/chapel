_extern def snprintf(buf: string, size: int, fmt: string, vals...?numvals): string;

def toString(fmt: string, x) {
    var buf: string = "12345678901234567890123456789012345678901234567890";
    snprintf(buf, 50, fmt, x);
    return buf;
}
