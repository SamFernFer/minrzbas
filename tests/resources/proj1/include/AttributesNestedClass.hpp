struct Upper {
    class
    [[clang::annotate("MyAttr(1)")]]
    [[clang::annotate("MyAttr(2)")]]
    [[clang::annotate("MyAttr(3)")]]
    NestedPublic {
        class InnerPrivate {
        };
    };
private:
    struct [[clang::annotate(R"(OtherAttr("Thing"))")]] Private {
    };
protected:
    class [[clang::annotate("Thing(0,1)")]] Protected {
    };
};