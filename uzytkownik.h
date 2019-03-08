#include <iostream>

using namespace std;

class Uzytkownik{

public:

    Uzytkownik(int=1, string="", string="");
    void wczytaj(int podaneID, string podanyLogin, string podaneHaslo);
    int zwrocID() const;
    string zwrocLogin() const;
    string zwrocHaslo() const;
    void zmienHaslo(string noweHaslo);

private:

    int id;
    string login, haslo;
};
