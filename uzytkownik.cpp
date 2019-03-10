#include <iostream>
#include "uzytkownik.h"
#include <fstream>
#include <cstdlib>
#include <windows.h>

using namespace std;

void podzielString(string linia, char znak, vector<string> &wyrazy){

    for(int i=0, j=0, dlugosc=linia.length(); j<=dlugosc; j++){
        if(linia[j] == znak){
            wyrazy.push_back(linia.substr(i,j-i));
            i = j+1;
        }
    }
}

//******************************************************************

Uzytkownik::Uzytkownik(int jeden, string dwa, string trzy){
    wczytaj(jeden, dwa, trzy);
}

void Uzytkownik::wczytaj(int podaneID, string podanyLogin, string podaneHaslo){
    if(podaneID >=1)
        id = podaneID;
    else
        cout << endl << "Niepoprawne id uzytkownika." << endl;

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

//***************************************************

Uzytkownicy::Uzytkownicy(string nazwaPliku){
    Uzytkownik uzytkownik;
    liczbaUzytkownikow = 0;
    idUzytkownika = 0;
    string linia="";
	fstream plik;
	vector<string> podzielonaLinia;

	plik.open(nazwaPliku.c_str(), ios::in);
    if(plik.good()==false){
        cout << "Nie udalo sie otworzyc pliku Uzytkownicy.txt. Zostanie utworzony nowy plik.";
        Sleep(1500);
        plik.open(nazwaPliku.c_str(), ios::out | ios::app);
        //exit(0);
    }
    else{
        while(getline(plik,linia)){

            podzielonaLinia.clear();
            podzielString(linia, '|', podzielonaLinia);

            uzytkownik.wczytaj(atoi (podzielonaLinia[0].c_str()), podzielonaLinia[1], podzielonaLinia[2]);

            uzytkownicy.push_back(uzytkownik);
            liczbaUzytkownikow++;
        }
    }
    plik.close();

}

int Uzytkownicy::zwrocLiczbeUzytkownikow() const{
    return liczbaUzytkownikow;
}

int Uzytkownicy::zwrocIdUzytkownika() const{
    return idUzytkownika;
}

void Uzytkownicy::zaloguj(){
string login="", haslo="";

    system("cls");
    cout << "Podaj login: " << endl;
    getline(cin, login);

    int i=0;
    while(i < liczbaUzytkownikow){
        if(uzytkownicy[i].zwrocLogin() == login){

            for(int j=0; j<3; j++){
                system("cls");
                cout << "Podaj haslo. Pozostalo prob: " << 3-j << endl;
                getline(cin, haslo);

                if(uzytkownicy[i].zwrocHaslo() == haslo){

                    idUzytkownika = uzytkownicy[i].zwrocID();
                    cout << "Zalogowales sie.";
                    Sleep(1000);
                    return;
                }
                cout << "Podane haslo jest nieprawidlowe.";
                Sleep(1500);
            }
            cout << endl << "Podales 3 razy bledne haslo.";
            Sleep(1500);
            return;
        }
        i++;
    }
    cout << "Brak w bazie uzytkownika o podanym loginie.";
    Sleep(1500);
    return;
}

void Uzytkownicy::wyloguj(){
    idUzytkownika = 0;
}

void Uzytkownicy::zarejestrujUzytkownika(string nazwaPliku){
    fstream plik;
    int id;
    string login="", haslo="";

    if(liczbaUzytkownikow > 0)
        id = uzytkownicy[liczbaUzytkownikow-1].zwrocID() + 1;
    else
        id = 1;

    system("cls");

    do{
        cout << "Podaj niepusta nazwe uzytkownika: " << endl;
        getline(cin, login);
    }while(login == "");

    int i=0;
    while(i < liczbaUzytkownikow){
        if(uzytkownicy[i].zwrocLogin() == login){
            cout << endl << "Istnieje juz uzytkownik o podanej nazwie. Podaj inna nazwe: " << endl;
            getline(cin, login);
            i=0;
        }
        else{
            i++;
        }
    }

    cout << "Podaj haslo uzytkownika: " << endl;
    getline(cin, haslo);

    Uzytkownik nowyUzytkownik(id, login, haslo);
    uzytkownicy.push_back(nowyUzytkownik);
    liczbaUzytkownikow++;

    plik.open(nazwaPliku.c_str(), ios::out | ios::app);
    if(plik.good())
        plik << id << "|" << login << "|" << haslo << "|" << endl;
    else{
        cout << "Nie udalo sie otworzyc pliku do zapisu. Program zostanie zamkniety.";
        exit(0);
    }
    plik.close();

    cout << "Nowy uzytkownik zostal zarejestrowany.";
    Sleep(1500);
}

void Uzytkownicy::zmienHaslo(string nazwaPliku){
    string haslo="";
    fstream plik;

    system("cls");
    cout << "Podaj nowe haslo: " << endl;
    getline(cin, haslo);

    for(int i=0; i<liczbaUzytkownikow; i++){
        if(uzytkownicy[i].zwrocID() == idUzytkownika){
            uzytkownicy[i].zmienHaslo(haslo);
            break;
        }
    }

    plik.open(nazwaPliku.c_str(), ios::trunc | ios::out);
    if(plik.good()==false){
        cout << "Nie udalo sie otworzyc pliku. Program zostanie zamkniety.";
        exit(0);
    }

    for(int i=0; i<liczbaUzytkownikow; i++){
         plik << uzytkownicy[i].zwrocID() << "|" << uzytkownicy[i].zwrocLogin() << "|" << uzytkownicy[i].zwrocHaslo()  << "|" << endl;
    }

    plik.close();

    cout << "Haslo zostalo zmienione.";
    Sleep(1500);
}
