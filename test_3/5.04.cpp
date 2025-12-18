#include <iostream>

//////////

template < typename Derived >
class Entity
{
public:
    void act()
    {
        static_cast<Derived*>(this)->act_impl();
    }
};

//////////

class Aggressive : public Entity<Aggressive>
{
public:
    void act_impl()
    {
        std::cout << "Aggressive action\n";
    }
};

//////////

class Defensive : public Entity<Defensive>
{
public:
    void act_impl()
    {
        std::cout << "Defensive action\n";
    }
};

//////////

int main()
{
    Aggressive a;
    Defensive d;

    a.act();
    d.act();
}
