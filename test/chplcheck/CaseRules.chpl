module CaseRules {
    var camelVar: int;
    var PascalVar: int;
    var snake_var: int;

    record camelRecord {
        proc camelCaseMethod() {return 0;}
        proc PascalCaseMethod() {return 0;}
        proc snake_case_method() {return 0;}
    }
    record PascalRecord {}
    record snake_record {
        var camelField, anotherCamelField: int;
        var camelField2, PascalField: real;
        var camelField3, snake_field: string;
    }

    proc camelProc() {return 0;}
    proc PascalProc() {return 0;}
    proc snake_proc() {
        var camelVar: int;
        var PascalVar: int;
        var snake_var: int;
        return 0;
    }

    proc foo(camelArg: int, PascalArg: string, snake_arg: real) {
        return camelArg + int(snake_arg) + PascalArg.size();
    }

    module camelModule {}
    module PascalModule {}
    module snake_module {}
    
    class camelClass {}
    class PascalClass {}
    class snake_class {}
}
