#include <iostream>

class Entity
{
public:
    virtual ~Entity() = default;

    virtual void draw() const = 0;
};

class SimpleEntity : public Entity
{
public:
    void draw() const override
    {
        std::cout << "SimpleEntity\n";
    }
};


template < typename T >
class Decorator
    : public Entity
    , public T
{
public:
    using T::T;

    void draw() const override
    {
        T::draw();
    }
};

class BorderDecorator : public Decorator<SimpleEntity>
{
public:
    using Decorator::Decorator;

    void draw() const override
    {
        std::cout << "Border start\n";
        SimpleEntity::draw();
        std::cout << "Border end\n";
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    BorderDecorator entity;
    entity.draw();

    return 0;
}
