namespace Root {
    namespace Named {
        namespace NS {
            namespace A {}
        }
    }
    namespace {
        namespace Hidden {
        }
    }
    namespace Named {
        namespace NS {
            namespace B {}
        }
    }
    namespace Named::NS {
        namespace C {
        }
    }
    namespace Named::Cat {
        namespace Dog {
        }
    }
    namespace {
        namespace Hidden {
            namespace Zero {
            }
        }
    }
}
namespace Other {
}