namespace Unions {
    union [[clang::annotate("Unions::MyUnion(0)")]] MyUnion;
    union [[clang::annotate("Unions::MyUnion(1)")]] MyUnion {
        char& printer;
    private:
        long myLong;
    };
}
union MyUnion {
    int intVal;
    long longVal;
protected:
    char charVal;
};
union [[clang::annotate("MyUnion(0)")]] MyUnion;
union [[clang::annotate("SimpleUnion(0)")]] SimpleUnion {};
union [[clang::annotate("Undefined(0)")]] Undefined;