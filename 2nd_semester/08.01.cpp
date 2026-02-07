
#include <cassert>

namespace semester2 {

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

int main()
{

    semester2::Entity_v2 entity_v2{};
    semester2::bad_reinterpret(entity_v2);

    assert(entity_v2.get_a() == 13);
}