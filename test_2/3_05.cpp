#include <iostream>

class Entity_v1
{
public:
    virtual void test()
    {
        std::cout << "Entity_v1::test()" << std::endl;
    }

    virtual ~Entity_v1() = default;
};

class Entity_v2
{
public:
    virtual void test()
    {
        std::cout << "Entity_v2::test()" << std::endl;
    }

    virtual ~Entity_v2() = default;
};

class Adapter_v1 : public Entity_v1 {
public:

    virtual void test_v1()
    {
        Entity_v1::test();
    }

    void test() override
    {
        test_v1();
    }
};

class Adapter_v2 : public Entity_v2 {
public:

    virtual void test_v2()
    {
        Entity_v2::test();
    }

    void test() override
    {
        test_v2();
    }
};

class Client : public Adapter_v1, public Adapter_v2 {
public:

    void call_v1_test()
    {
        Adapter_v1::test_v1();
    }

    void call_v2_test()
    {
        Adapter_v2::test_v2();
    }

    void call_original_v1_test()
    {
        Entity_v1::test();
    }

    void call_original_v2_test()
    {
        Entity_v2::test();
    }
};

int main()
{
    Client client;

    client.call_v1_test();
    client.call_v2_test();

    client.call_original_v1_test();
    client.call_original_v2_test();

    return 0;
}
