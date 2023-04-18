// this test does not have any concurrency.
// it only checks that the various operations on shared classes and weakPointers work correctly together

use WeakPointer;

class basicClass {
    var x :int;
}

var bc_weak1;
{
    writeln("1");

    var bc1 = new shared basicClass(1);
    info(bc1);

    bc_weak1 = new weak(bc1);
    weak_info(bc_weak1);

    var bc_weak2;
    {
        bc1.x += 1;
        writeln("2");

        var bc2 = bc1;

        bc_weak2 = new weak(bc2);

        info(bc1);
        info(bc2);

        weak_info(bc_weak1);
        weak_info(bc_weak2);

        var bc_weak3;
        {
            bc2.x += 1;
            writeln("3");

            var bc3 = bc1;

            // weak pointer via initializer
            bc_weak3 = bc3.downgrade();

            info(bc1);
            info(bc2);
            info(bc3);

            weak_info(bc_weak1);
            weak_info(bc_weak2);
            weak_info(bc_weak3);

            var bc_weak4;
            {
                bc1.x += 1;
                writeln("4");

                // copy init weak pointer
                bc_weak4 = bc_weak2;

                // create a shared by casting directly to a non-nilable class
                var bc4 = new shared basicClass();
                try {
                    bc4 = bc_weak1 : shared basicClass;
                } catch e {
                    writeln(e);
                }

                info(bc1);
                info(bc2);
                info(bc3);
                info(bc4);

                weak_info(bc_weak1);
                weak_info(bc_weak2);
                weak_info(bc_weak3);
                weak_info(bc_weak4);
            }
            bc3.x -= 1;
            writeln("3");

            info(bc1);
            info(bc2);
            info(bc3);

            weak_info(bc_weak1);
            weak_info(bc_weak2);
            weak_info(bc_weak3);
            weak_info(bc_weak4);

        }
        bc2.x -= 1;
        writeln("2");

        info(bc1);
        info(bc2);

        weak_info(bc_weak1);
        weak_info(bc_weak2);
        weak_info(bc_weak3);
    }
    bc1.x -= 1;
    writeln("1");

    info(bc1);
    weak_info(bc_weak1);
}
writeln("0");
weak_info(bc_weak1);

// can't upgrade because there are no shared references still alive
var bc_failed_upgrade = bc_weak1 : shared basicClass?;
writeln("upgrade attempt: ", bc_failed_upgrade);

proc info(x) {
    writeln("\tshared: '", x, "' \ttype: '", x.type:string, "' \t\tsc: ", x.chpl_pn!.strongCount.read());
}

proc weak_info(x) {
    writeln("\tweak: '", x, "' \ttype: '", x.type:string, "' \tsc: ", x.getStrongCount(),  "\t wc: ", x.getWeakCount());
}
