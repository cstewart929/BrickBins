using namespace std;
#include <iostream>
#include "Kit.h"

void Kit::setName(const std::string &name)
{
    name_ = name;
}
const std::string &Kit::getName() const
{
    return name_;
}

void Kit::setTheme(const std::string &theme)
{
    theme_ = theme;
}
const std::string &Kit::getTheme() const
{
    return theme_;
}

void Kit::setId(const int &id)
{
    id_ = id;
}
const int &Kit::getId() const
{
    return id_;
}

void Kit::setPieces(const int &pieces)
{
    pieces_ = pieces;
}
const int &Kit::getPieces() const
{
    return pieces_;
}

void Kit::setMinifigs(const int &minifigs)
{
    minifigs_ = minifigs;
}
const int &Kit::getMinifigs() const
{
    return minifigs_;
}

void Kit::setYear(const int &year)
{
    year_ = year;
}
const int &Kit::getYear() const
{
    return year_;
}
