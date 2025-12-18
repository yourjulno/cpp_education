#include <iostream>
#include <memory>
#include <vector>
#include <string>

//////////

class GameObject
{
public:
    virtual ~GameObject() = default;

    virtual void update() = 0;

    void tick()
    {
        pre_update();
        update();
        post_update();
    }

protected:
    virtual void pre_update()
    {
    }

    virtual void post_update()
    {
    }
};

//////////

class CompositeObject : public GameObject
{
public:
    void add(std::shared_ptr<GameObject> obj)
    {
        children_.push_back(obj);
    }

    void update() override
    {
        for (auto& c : children_)
        {
            c->tick();
        }
    }

private:
    std::vector<std::shared_ptr<GameObject>> children_;
};

//////////

class Unit : public GameObject
{
public:
    explicit Unit(std::string name)
        : name_(std::move(name))
    {
    }

    void update() override
    {
        std::cout << "Unit " << name_ << " acts\n";
    }

private:
    std::string name_;
};

//////////

class GameBuilder
{
public:
    GameBuilder()
        : root_(std::make_shared<CompositeObject>())
    {
    }

    GameBuilder& add_unit(const std::string& name)
    {
        root_->add(std::make_shared<Unit>(name));
        return *this;
    }

    std::shared_ptr<GameObject> get()
    {
        return root_;
    }

private:
    std::shared_ptr<CompositeObject> root_;
};

//////////

int main()
{
    GameBuilder builder;

    auto game =
        builder
            .add_unit("A")
            .add_unit("B")
            .get();

    game->tick();
}
