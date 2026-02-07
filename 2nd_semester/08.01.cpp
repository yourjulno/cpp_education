
#include <cassert>

namespace semester2::first {

class Entity_v2
{
private:
    int a = 10;

public:

    int get_a() const
    {
        return a;
    }
};

class Entity_v1
{
public:
    int b = 12;
};

void bad_reinterpret(Entity_v2 & entity_v2)
{
    (reinterpret_cast<Entity_v1 &>(entity_v2)).b = 13;
}

}

namespace semester2::first2 {

#define private public

class Entity_v2
{
private:
    int a = 10;

public:

    int get_a() const
    {
        return a;
    }
};

void bad_define(Entity_v2 & entity_v2)
{
    entity_v2.a = 13;
}

}

int main()
{

    semester2::first::Entity_v2 entity_v2{};
    semester2::first::bad_reinterpret(entity_v2);
    assert(entity_v2.get_a() == 13);

    semester2::first2::Entity_v2 entity_v2_define{};
    semester2::first2::bad_define(entity_v2_define);
    assert(entity_v2_define.get_a() == 13);
}