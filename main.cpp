using namespace std;
#include <iostream>
#include <string>
#include "Kit.h"

int main()
{
    Kit kit;
    kit.setId(10254);
    kit.setName("Winter Holiday Train");
    kit.setTheme("Winter Village");
    kit.setYear(2016);
    kit.setPieces(713);
    kit.setMinifigs(5);

    cout << "ID: " << kit.getId() << endl;
    cout << "Name: " << kit.getName() << endl;
    cout << "Theme: " << kit.getTheme() << endl;
    cout << "Year: " << kit.getYear() << endl;
    cout << "Pieces: " << kit.getPieces() << endl;
    cout << "Minifigs: " << kit.getMinifigs() << endl;
    return 0;
};
