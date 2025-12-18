template <typename T, T v>
struct integral_constant
{
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant;

    constexpr operator value_type() const noexcept
    {
        return value;
    }
};

using true_type  = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template <typename T>
struct is_class
{
private:
    template <typename U>
    static char test(int U::*);

    template <typename U>
    static int test(...);

public:
    static constexpr bool value =
        sizeof(test<T>(0)) == sizeof(char);
};

template <typename T>
inline constexpr bool is_class_v = is_class<T>::value;

template <typename T>
struct add_const
{
    using type = const T;
};

template <typename T>
using add_const_t = typename add_const<T>::type;

template <typename T>
struct remove_const
{
    using type = T;
};

template <typename T>
struct remove_const<const T>
{
    using type = T;
};

template <typename T>
using remove_const_t = typename remove_const<T>::type;

template <bool B, typename T, typename F>
struct conditional
{
    using type = T;
};

template <typename T, typename F>
struct conditional<false, T, F>
{
    using type = F;
};

template <bool B, typename T, typename F>
using conditional_t = typename conditional<B, T, F>::type;

// remove_reference

template <typename T>
struct remove_reference
{
    using type = T;
};

template <typename T>
struct remove_reference<T&>
{
    using type = T;
};

template <typename T>
struct remove_reference<T&&>
{
    using type = T;
};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

// is_array

template <typename T>
struct is_array : false_type
{
};

template <typename T, unsigned N>
struct is_array<T[N]> : true_type
{
};

template <typename T>
struct is_array<T[]> : true_type
{
};

// remove_extent

template <typename T>
struct remove_extent
{
    using type = T;
};

template <typename T, unsigned N>
struct remove_extent<T[N]>
{
    using type = T;
};

template <typename T>
struct remove_extent<T[]>
{
    using type = T;
};


template <typename T>
struct decay
{
private:
    using U = remove_reference_t<T>;

public:
    using type =
        conditional_t<
            is_array<U>::value,
            typename remove_extent<U>::type*,
            remove_const_t<U>
        >;
};

template <typename T>
using decay_t = typename decay<T>::type;

struct TestClass
{
};

union TestUnion
{
    int x;
};

int main()
{
    static_assert(is_class<TestClass>::value, "TestClass is class");
    static_assert(!is_class<int>::value, "int is not class");

    static_assert(remove_const_t<const int>{} == 0, "");

    static_assert(
            sizeof(conditional_t<true, int, double>) ==
            sizeof(int),
            "conditional true"
    );

    static_assert(
            sizeof(conditional_t<false, int, double>) ==
            sizeof(double),
            "conditional false"
    );

    static_assert(
            sizeof(decay_t<int &>) == sizeof(int),
            "decay reference"
    );

    static_assert(
            sizeof(decay_t<const int>) == sizeof(int),
            "decay const"
    );

    static_assert(
            sizeof(decay_t<int[3]>) == sizeof(int *),
            "decay array"
    );
}
