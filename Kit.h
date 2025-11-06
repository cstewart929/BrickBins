#ifndef KIT_H
#define KIT_H

#include <string>

class Kit
{
public:
    Kit() = default;
    ~Kit() = default;

    void setTheme(const std::string &theme);
    const std::string &getTheme() const;

    void setName(const std::string &name);
    const std::string &getName() const;

    void setId(const int &id);
    const int &getId() const;

    void setPieces(const int &pieces);
    const int &getPieces() const;

    void setMinifigs(const int &minifigs);
    const int &getMinifigs() const;

    void setYear(const int &year);
    const int &getYear() const;

private:
    std::string name_;
    std::string theme_;
    int id_;
    int pieces_;
    int minifigs_;
    int year_;
};

#endif
