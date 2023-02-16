param ToLookFor = "??";
module M1 {
    module ToLookFor {}
    module M2 {
        module ToLookFor {}
        module M3 {
            module ToLookFor {}
            module M4 {
                use ToLookFor;
                var x = ToLookFor.x;
            }
        }
    }
}
