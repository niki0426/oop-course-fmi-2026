#include "Order.h"
#include "Table.h"
#include "Dish.h"

#include <algorithm>
#include <numeric>

unsigned Order::nextOrderId = 1;

Order::Order(const std::shared_ptr<Table>& table)
    : id(nextOrderId++), table(table)
{
}

unsigned Order::getId() const
{
    return id;
}

bool Order::isClosed() const
{
    return closed;
}

bool Order::isValid() const
{
    return !table.expired();
}

void Order::close()
{
    closed = true;
}

void Order::addDish(const std::shared_ptr<Dish>& dish, unsigned quantity)
{
    if (closed || !dish || quantity == 0)
    {
        return;
    }

    for (unsigned i = 0; i < quantity; i++)
    {
        dishes.push_back(dish);
    }
}

bool Order::removeOneDish(const std::string& dishName)
{
    auto it = std::find_if(dishes.begin(), dishes.end(),
        [&](const std::weak_ptr<Dish>& weakDish)
        {
            auto dish = weakDish.lock();
            return dish && dish->getName() == dishName;
        });

    if (it == dishes.end())
    {
        return false;
    }

    dishes.erase(it);
    return true;
}

void Order::removeInvalidDishes()
{
    std::erase_if(dishes,
                  [](const std::weak_ptr<Dish>& weakDish)
                  {
                      return weakDish.expired();
                  });
}

double Order::getTotal() const
{
    return std::accumulate(dishes.begin(), dishes.end(), 0.0,
        [](double sum, const std::weak_ptr<Dish>& weakDish)
        {
            auto dish = weakDish.lock();
            if (!dish)
            {
                return sum;
            }

            return sum + dish->getPrice();
        });
}

std::shared_ptr<Table> Order::getTable() const
{
    return table.lock();
}

std::size_t Order::getDishCount() const
{
    return static_cast<std::size_t>(
        std::count_if(dishes.begin(), dishes.end(),
            [](const std::weak_ptr<Dish>& weakDish)
            {
                return !weakDish.expired();
            })
    );
}

bool Order::containsDish(const std::string& dishName) const
{
    return std::any_of(dishes.begin(), dishes.end(),
        [&](const std::weak_ptr<Dish>& weakDish)
        {
            auto dish = weakDish.lock();
            return dish && dish->getName() == dishName;
        });
}

std::ostream& operator<<(std::ostream& os, const Order& order)
{
    os << "Order #" << order.id << " | ";

    auto table = order.getTable();
    if (table)
    {
        os << "Table: " << table->getNumber();
    }
    else
    {
        os << "Table: <expired>";
    }

    os << " | Status: " << (order.closed ? "closed" : "open")
       << " | Valid dishes: " << order.getDishCount()
       << " | Total: " << order.getTotal()
       << " | Items: ";

    bool first = true;
    for (const auto& weakDish : order.dishes)
    {
        auto dish = weakDish.lock();
        if (!dish)
        {
            continue;
        }

        if (!first)
        {
            os << ", ";
        }

        os << dish->getName();
        first = false;
    }

    if (first)
    {
        os << "<none>";
    }

    return os;
}
