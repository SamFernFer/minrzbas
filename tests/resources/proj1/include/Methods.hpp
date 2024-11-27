#ifndef FENTON_METHODS_HPP
#define FENTON_METHODS_HPP

namespace ThingNS {
    struct Base {
        virtual long getLong() const = 0;
        virtual char getChar();
        virtual const char* getStr();
    };
    struct Thing1 : Base {
        static void function() noexcept { }
private:
        static Thing1 New(long myLong);
protected:
        long getLong() const override;
        // Implicitly overriding.
        virtual char getChar();
        // Overring a non-pure virtual function.
        const char* getStr() override;

        void doThing(float f, double d, long long ll = 42);
    };
    Thing1 Thing1::New(long myLong) {
        return {};
    }
}

#endif