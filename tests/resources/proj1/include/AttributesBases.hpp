namespace Space {
    struct Base {
    };
    class InnerBase {
    };
    class SecondBase : [[clang::annotate("MyAttr(1)")]] InnerBase {
    };

    struct Derived : 
        [[clang::annotate("MyAttr(2)")]] protected Base,
        [[clang::annotate("MyAttr(3)")]]SecondBase {
        
    };
}