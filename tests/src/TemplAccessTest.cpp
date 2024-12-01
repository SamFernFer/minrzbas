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

    template<typename Tag> struct FuncHolder {
        // inline static std::list<std::functon<void()>> funcs;
        inline static void(*f)();
    };
    template<typename Tag> struct TestHolder {
        inline static void(*f)();
    };
    template<typename Tag, typename T> struct TemplTest {
        // struct Reg {
        //     Reg() {
        //         TestHolder<Tag>::f = T::test;
        //     }
        // };
        // static Reg reg;
        TemplTest() {
            // FuncHolder<Tag>::funcs.emplace_back(T::test);
            FuncHolder<Tag>::f = &T::test;
        }
        // static volatile TemplTest<Tag, T> ins;
    };
    template<typename Tag, typename T> TemplTest<Tag, T> templVar;

    struct Tag1 {};
    struct Tag2 {};

    template<> TemplTest<Tag1, Public> templVar<Tag1, Public>;
    template<> TemplTest<Tag2, Outer::Inner> templVar<Tag2, Outer::Inner>;

    // template<> Ret<Outer::Inner, Public> var;

    // template<> volatile TemplTest<Tag1, Public> TemplTest<Tag1, Public>::ins;
    // template<> volatile TemplTest<Tag2, Outer::Inner> TemplTest<Tag2, Outer::Inner>::ins;

    void templ_access() {
        FuncHolder<Tag1>::f();
        FuncHolder<Tag2>::f();
    }
}