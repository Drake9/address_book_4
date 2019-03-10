#include <iostream>
#include <vector>

using namespace std;

void podzielString(string linia, char znak, vector<string> &wyrazy);

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

class Uzytkownicy :public Uzytkownik{

public:

    Uzytkownicy(string nazwaPliku = "Uzytkownicy.txt");
    int zwrocIdUzytkownika() const;
    int zwrocLiczbeUzytkownikow() const;
    void zaloguj();
    void wyloguj();
    void zarejestrujUzytkownika(string nazwaPliku);
    void zmienHaslo(string nazwaPliku);

private:

    int idUzytkownika, liczbaUzytkownikow;
    vector <Uzytkownik> uzytkownicy;
};
