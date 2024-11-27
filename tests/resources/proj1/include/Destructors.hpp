class Class {
    ~Class();
public:
    ~Class(int i, long l) noexcept;
protected:
    ~Class(double a, double b);
private:
    ~Class(float a, const Class* parentClass, long myLong = 90);
};