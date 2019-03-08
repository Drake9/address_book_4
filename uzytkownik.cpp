#include <iostream>
#include "uzytkownik.h"
#include <fstream>
#include <cstdlib>

using namespace std;

Uzytkownik::Uzytkownik(int jeden, string dwa, string trzy){
    wczytaj(jeden, dwa, trzy);
}

void Uzytkownik::wczytaj(int podaneID, string podanyLogin, string podaneHaslo){
    if(podaneID >=1)
        id = podaneID;
    else
        cout << endl << "ID uzytkownika nie moze byc ujemne!" << endl;

    login = podanyLogin;
    haslo = podaneHaslo;
}

int Uzytkownik::zwrocID() const{
    return id;
}

string Uzytkownik::zwrocLogin() const{
    return login;
}

string Uzytkownik::zwrocHaslo() const{
    return haslo;
}

void Uzytkownik::zmienHaslo(string noweHaslo){
    haslo = noweHaslo;
}
