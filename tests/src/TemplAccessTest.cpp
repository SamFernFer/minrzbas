#include <iostream>
#include <list>
#include <functional>

namespace Fenton::Minrzbas::Tests {
    class Outer {
        struct Inner {
            static void test() {
                std::cout << "It works!" << std::endl;
            }
        };
    };
    struct Public {
        static void test() {
            std::cout << "Just testing." << std::endl;
        }
    };

    struct FuncHolder {
        inline static std::list<std::function<void()>> funcs;
    };
    template<typename T> struct TemplTest {
        struct Reg {
            Reg() {
                FuncHolder::funcs.emplace_back(T::test);
            }
        };
        inline static Reg reg;
    };
    template<int I> struct Wrapper {
        using type = int;
    };

    // template<> TemplTest<Outer::Inner>::Reg TemplTest<Outer::Inner>::reg;

    template<> using Wrapper<0>::type = TemplTest<Outer::Inner>::reg;

    void templ_access() {
        for (auto& f : FuncHolder::funcs) {
            f();
        }
    }
}