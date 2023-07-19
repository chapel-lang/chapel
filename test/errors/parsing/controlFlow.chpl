break someLabel;
continue someLabel;

proc wrapper() {
    break someLabel;
    continue someLabel;
}

break;
continue;
return;
yield 1;

proc wrapper2() {
    break;
    continue;
}

// Each of the following block continues and returns.
forall 1..10 {
    break;
    break someLabel;
    continue;
    continue someLabel;
}
foreach 1..10 {
    break;
    break someLabel;
    continue;
    continue someLabel;
}
coforall 1..10 {
    break;
    break someLabel;
    continue;
    continue someLabel;
}
on here {
    break;
    break someLabel;
    continue;
    continue someLabel;
}
begin {
    break;
    break someLabel;
    continue;
    continue someLabel;
}
sync {
    break;
    break someLabel;
    continue;
    continue someLabel;
}
cobegin {
    break;
    break someLabel;
    continue;
    continue someLabel;
}

label actualLabel for 1..10 {
    proc wrapper2() {
        break;
        break actualLabel;
        continue;
        continue actualLabel;
    }

    // Each of the following block continues and returns.
    forall 1..10 {
        break;
        break actualLabel;
        continue;
        continue actualLabel;
    }
    foreach 1..10 {
        break;
        break actualLabel;
        continue;
        continue actualLabel;
    }
    coforall 1..10 {
        break;
        break actualLabel;
        continue;
        continue actualLabel;
    }
    on here {
        break;
        break actualLabel;
        continue;
        continue actualLabel;
    }
    begin {
        break;
        break actualLabel;
        continue;
        continue actualLabel;
    }
    sync {
        break;
        break actualLabel;
        continue;
        continue actualLabel;
    }
    cobegin {
        break;
        break actualLabel;
        continue;
        continue actualLabel;
    }
}
