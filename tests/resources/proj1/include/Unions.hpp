namespace Unions {
    union MyUnion;
    union MyUnion {
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
union MyUnion;
union SimpleUnion {};
union Undefined;