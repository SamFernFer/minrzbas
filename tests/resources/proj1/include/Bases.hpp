namespace Space {
    struct Base {
    };
    class InnerBase {
    };
    class SecondBase : InnerBase {
    };

    struct Derived : protected Base, SecondBase {
        
    };
}