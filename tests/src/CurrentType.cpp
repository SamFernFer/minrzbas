#include <utils/Misc.hpp>
#include <string>
#include <type_traits>

class MyClass;

template<typename T> std::string typeName = "?";
template<> std::string typeName<MyClass> = "MyClass";

template<typename T> class Dummy {

};

class MyClass {
    int var;
public:
    auto currType()->decltype(this)* {
        return nullptr;
    }
    std::string type = typeName<std::remove_pointer_t<decltype(this)>>;
};
struct MyStruct {
    int var;
};

template<typename T> concept Accessable = requires(T a) {
    { a.var };
};

// The consteval function should leave behind no traces of itself, making the debug executable 
// smaller.
template<typename T> consteval bool compiles() {
    return requires { { T::var }; };
}

namespace Fenton::Minrzbas::Tests {
    bool current_type() {
        Fenton::printlnf("Accessable(MyClass) = {}", compiles<MyClass>()? "true" : "false");
        Fenton::printlnf("Accessable(MyStruct) = {}", compiles<MyStruct>()? "true" : "false");
        MyClass obj = MyClass();
        bool _pass = obj.type == "MyClass";
        // if (!_pass) {
        //     Fenton::printlnf("[Actual] Type: {}", obj.type);
        // }
        // Fenton::printlnf("[{}] " "Current Type", _pass? "PASS" : "FAIL");
        return _pass;
    }
}