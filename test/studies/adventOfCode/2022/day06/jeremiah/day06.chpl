use IO;

config param stackSize = 4;

var sequence: bytes,
    stack: sequenceStack(stackSize);

stdin.readAll(sequence);

for (byte, idx) in zip(sequence, 1..) {
    stack.push(byte);
    if !stack.hasRepeats() && idx > stackSize {
        writeln(idx);
        break;
    }
}

record sequenceStack {
    param size: int;
    var chars: [1..size] uint(8);
}

proc sequenceStack.push(c: uint(8)) {
    for param i in 1..<this.size do
        this.chars[i] = chars[i+1];
    this.chars[this.size] = c;
}

proc sequenceStack.hasRepeats(): bool {
    for param i in 1..this.size do
        for param j in (i+1)..this.size do
            if this.chars[i] == this.chars[j]
                then return true;
    return false;
}
