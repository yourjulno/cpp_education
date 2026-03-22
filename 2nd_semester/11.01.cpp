#include <iostream>
#include <type_traits>

class Wrapper {
public:
    using Fn = void (*)();

    explicit Wrapper(Fn fn) : fn_(fn) {}

    Fn operator*() const noexcept { return fn_; }
    operator Fn() const noexcept { return fn_; }

private:
    Fn fn_{nullptr};
};

void test_fn()
{
    std::cout << "test_fn\n";
}

Wrapper test()
{
    return Wrapper(&test_fn);
}

int main()
{
    static_assert(std::is_same_v<decltype(test_fn), void()>);
    static_assert(std::is_same_v<decltype(&test_fn), void (*)()>);

    Wrapper function = test();
    (*function)();

    Wrapper::Fn fp = function;
    fp();

    return 0;
}