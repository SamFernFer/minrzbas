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

union [[clang::annotate("0"), clang::annotate("0.0")]] RedeclaredUnion;
union [[clang::annotate("1")]] RedeclaredUnion {
};
union [[clang::annotate("2")]] RedeclaredUnion;
union [[clang::annotate("3")]] RedeclaredUnion;