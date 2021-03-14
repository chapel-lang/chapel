const D1: domain(1, stridable=true) = {1..10 by 2};
const D2: domain(1, stridable=true) = {1..9 by 2};
const D3: domain(1, stridable=true) = {1..10 by 2 by -1};
const D4: domain(1, stridable=true) = {1..9 by -2};

writeln("{1..10 by 2} =? {1..9 by 2}: ", D1 == D2);

writeln("{1..10 by 2 by -1} =? {1..9 by -2}: ", D3 == D4);

writeln("{1..9 by 2} =? {1..9 by -2}: ", D2 == D4);
