
proc mightThrow(i: int) throws {
    if i%2==0 then throw new Error("error!");
}

try {
    coforall tid in 0..#5 {
        mightThrow(tid);
    }
} catch e: TaskErrors {
    writeln(e.first());
} catch e { }
