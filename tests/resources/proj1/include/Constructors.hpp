struct Struct {
    Struct(long l);
    explicit Struct(double d);
protected:
    Struct(char a, char b);
private:
    Struct(const char* const str = "ERROR");
public:
    Struct();
    Struct(Struct&&) = default;
    Struct(const Struct&) = delete;
    Struct(double a, double b, double c = 0.0, double d = 10.0);
}
Struct::Struct(long l) {}