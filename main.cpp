#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include "uzytkownik.h"

using namespace std;

const string nazwaPlikuZUzytkownikami = "Uzytkownicy.txt";
const string nazwaPlikuZAdresatami = "Adresaci.txt";
const string nazwaTymczasowegoPlikuZAdresatami = "Adresaci_tymczasowy.txt";

struct Adresat{
    int idAdresata = 0, idUzytkownika = 0;
    string imie = "", nazwisko = "", numerTelefonu = "", email = "", adres = "";
};

int wczytajAdresatowZPliku(vector<Adresat> &adresy, int idUzytkownika, int &idOstatniegoAdresata){
    int nrKontaktu = 0;
	Adresat kontakt;
	string linia;
	fstream plik;
	vector<string> podzielonaLinia;

    plik.open(nazwaPlikuZAdresatami.c_str(), ios::in); // wczytujemy plik
    if(plik.good()==false){
        cout << endl <<"Nie udalo sie otworzyc pliku z kontaktami. Zostanie utworzony nowy plik.";
        Sleep(1500);
        plik.open(nazwaPlikuZAdresatami.c_str(), ios::out | ios::app);
    }
    else{
        while(getline(plik,linia)){

            podzielonaLinia.clear();
            podzielString(linia, '|', podzielonaLinia);

            if(atoi(podzielonaLinia[1].c_str()) == idUzytkownika){

                kontakt.idAdresata = atoi(podzielonaLinia[0].c_str());
                kontakt.idUzytkownika = atoi(podzielonaLinia[1].c_str());
                kontakt.imie = podzielonaLinia[2];
                kontakt.nazwisko = podzielonaLinia[3];
                kontakt.numerTelefonu = podzielonaLinia[4];
                kontakt.email = podzielonaLinia[5];
                kontakt.adres = podzielonaLinia[6];

                adresy.push_back(kontakt);

            }
            nrKontaktu++;
            idOstatniegoAdresata = atoi(podzielonaLinia[0].c_str());
        }
    }
    plik.close();

    return nrKontaktu;
}

int dodajKontakt(int idOstatniegoKontaktu, vector<Adresat> &adresy, int idUzytkownika){
    Adresat kontakt;
    fstream plik;
    kontakt.idAdresata = idOstatniegoKontaktu + 1;

    kontakt.idUzytkownika = idUzytkownika;

    system("cls");
    cin.sync();

    do{
        cout << "Podaj imie : ";            getline(cin, kontakt.imie);
    }while(kontakt.imie == "");

    do{
        cout << "Podaj nazwisko : ";        getline(cin, kontakt.nazwisko);
    }while(kontakt.nazwisko == "");

    cout << "Podaj adres: ";          getline(cin, kontakt.adres);
    cout << "Podaj nr telefonu: ";    getline(cin, kontakt.numerTelefonu);
    cout << "Podaj adres e-mail: ";   getline(cin, kontakt.email);

    adresy.push_back(kontakt);

    plik.open(nazwaPlikuZAdresatami.c_str(), ios::out | ios::app);
    if(plik.good())
        plik << kontakt.idAdresata << "|" << kontakt.idUzytkownika << "|" << kontakt.imie << "|" << kontakt.nazwisko << "|" << kontakt.numerTelefonu << "|" << kontakt.email << "|" << kontakt.adres << "|" << endl;
    else{
        cout << "Nie udalo sie otworzyc pliku do zapisu. Program zostanie zamkniety.";
        exit(0);
    }
    plik.close();

    return kontakt.idAdresata;
}

void wypiszKontakt(int numerKontaktu, vector<Adresat> &adresy){
	cout << adresy[numerKontaktu].idAdresata  << endl;
	cout << adresy[numerKontaktu].imie << " ";
	cout << adresy[numerKontaktu].nazwisko << endl;
	cout << adresy[numerKontaktu].numerTelefonu << endl;
	cout << adresy[numerKontaktu].email << endl;
	cout << adresy[numerKontaktu].adres << endl << endl;
}

void wyszukajKontaktPoImieniu(vector<Adresat> &adresy){
	int sprawdzono = 0;
	string imie = "";
	int rozmiar = adresy.size();

	system("cls");
	cout << "Wpisz imie: ";
	getline(cin, imie);
	system("cls");

	for(int i=0; i<rozmiar; i++){
		if(adresy[i].imie==imie){
			wypiszKontakt(i, adresy);
		}
		else{
			sprawdzono++;
		}
	}

	if(sprawdzono==rozmiar){
		cout << "Nie znaleziono pasujacych wynikow." << endl << endl;
	}

	system("pause");
}

void wyszukajKontaktPoNazwisku(vector<Adresat> &adresy){
	int sprawdzono = 0;
	string nazwisko = "";
	int rozmiar=adresy.size();

	system("cls");
	cout << "Wpisz nazwisko: ";
	getline(cin, nazwisko);
	system("cls");

	for(int i=0; i<rozmiar; i++){
		if(adresy[i].nazwisko==nazwisko){
			wypiszKontakt(i, adresy);
		}
		else{
			sprawdzono++;
		}
	}
	if(sprawdzono==rozmiar){
		cout << "Nie znaleziono pasujacych wynikow." << endl << endl;
	}

	system("pause");
}

void wypiszWszystkieKontakty(vector<Adresat> &adresy){
	system("cls");
	int liczbaAdresatow = adresy.size();
	cout << "Liczba Kontaktow: " << liczbaAdresatow << endl << endl;
	for(int i=0; i<liczbaAdresatow; i++){
		wypiszKontakt(i, adresy);
	}
	system("pause");
}

int wyszukajKontaktPoID(int id, vector<Adresat> &adresy){

    for(int index=0, rozmiar=adresy.size(); index<rozmiar; index++){
        if(adresy[index].idAdresata == id)
            return index;
    }

    return -2;
}

void usunAdresata(vector<Adresat> &adresy, int index, int wybraneID, int &idOstatniegoAdresata){
    fstream staryPlik, nowyPlik;
    string linia;
    vector <string> podzielonaLinia;

    adresy.erase(adresy.begin() + index);

    staryPlik.open(nazwaPlikuZAdresatami.c_str(), ios::in);
    nowyPlik.open(nazwaTymczasowegoPlikuZAdresatami.c_str(), ios::out | ios::app);
    if(staryPlik.good()==false || nowyPlik.good()==false){
        cout << "Nie udalo sie otworzyc pliku. Program zostanie zamkniety.";
        exit(0);
    }

    while(getline(staryPlik, linia)){

        podzielonaLinia.clear();
        podzielString(linia, '|', podzielonaLinia);

        if(atoi(podzielonaLinia[0].c_str()) != wybraneID){
            nowyPlik << linia << endl;
            idOstatniegoAdresata = atoi(podzielonaLinia[0].c_str());
        }
    }

    staryPlik.close();
    if( remove( nazwaPlikuZAdresatami.c_str() ) != 0 ){
        cout << endl << "Nie powiodlo sie usuniecie pliku. Program zostanie zamkniety." << endl;
        exit(0);
    }

    nowyPlik.close();
    if( rename( nazwaTymczasowegoPlikuZAdresatami.c_str(), nazwaPlikuZAdresatami.c_str() ) != 0 ){
        cout << endl << "Nie powiodla sie zmiana nazwy pliku. Program zostanie zamkniety." << endl;
        exit(0);
    }
}

void przejdzDoUsuwaniaAdresata(int &idOstatniegoAdresata, vector<Adresat> &adresy){
    string wybraneIDJakoString = "";
    char wybor = '0';
    int wybraneID = 0;
    system("cls");
    cout << "Wpisz id adresata, ktorego chcesz usunac: ";

    getline(cin, wybraneIDJakoString);
    wybraneID = atoi(wybraneIDJakoString.c_str());
    int index = wyszukajKontaktPoID(wybraneID, adresy);

    if(index >= 0 && index < idOstatniegoAdresata){
        cout << endl << "Czy na pewno chcesz usunac adresata: " << adresy[index].imie << " " << adresy[index].nazwisko << " ?" << endl << endl;
        cout << "Aby usunac wybranego adresata, nacisnij: t" << endl << endl;
        cout << "Aby powrocic do menu, nacisnij dowolny inny klawisz." << endl;

        wybor = getch();
        if(wybor == 't' || wybor == 'T'){
            usunAdresata(adresy, index, wybraneID, idOstatniegoAdresata);
        }
    }
    else{
        cout << "Nie znaleziono adresata o podanym id." << endl;
        Sleep(1500);
    }
}

void edytujAtrybutAdresata(int index, int wybraneID, vector<Adresat> &adresy, string &atrybut){
    fstream staryPlik, nowyPlik;
    string linia;
    vector <string> podzielonaLinia;

    system("cls");
    cout << "Podaj nowy atrybut adresata: " << endl;
    getline(cin, atrybut);

    staryPlik.open(nazwaPlikuZAdresatami.c_str(), ios::in);
    nowyPlik.open(nazwaTymczasowegoPlikuZAdresatami.c_str(), ios::out | ios::app);
    if(staryPlik.good()==false || nowyPlik.good()==false){
        cout << "Nie udalo sie otworzyc pliku. Program zostanie zamkniety.";
        exit(0);
    }

    while(getline(staryPlik, linia)){

        podzielonaLinia.clear();
        podzielString(linia, '|', podzielonaLinia);

        if(atoi(podzielonaLinia[0].c_str()) == wybraneID){
            nowyPlik << adresy[index].idAdresata << "|" << adresy[index].idUzytkownika << "|" << adresy[index].imie << "|" << adresy[index].nazwisko <<  "|" << adresy[index].numerTelefonu << "|" << adresy[index].email << "|" << adresy[index].adres << "|" << endl;
        }
        else{
            nowyPlik << linia << endl;
        }
    }

    staryPlik.close();
    if( remove( nazwaPlikuZAdresatami.c_str() ) != 0 ){
        cout << endl << "Nie powiodlo sie usuniecie pliku. Program zostanie zamkniety." << endl;
        exit(0);
    }

    nowyPlik.close();
    if( rename( nazwaTymczasowegoPlikuZAdresatami.c_str(), nazwaPlikuZAdresatami.c_str() ) != 0 ){
        cout << endl << "Nie powiodla sie zmiana nazwy pliku. Program zostanie zamkniety." << endl;
        exit(0);
    }
}

void edytujAdresata(int index, int id, vector<Adresat> &adresy){
    char wybor = '0';
    cout << endl << "Edycja adresata: " << endl << endl;
    wypiszKontakt(index, adresy);
    cout << "Wybierz, ktory atrybut chcesz zmienic." << endl << endl;
    cout << "1 - imie" << endl;
    cout << "2 - nazwisko" << endl;
    cout << "3 - numer telefonu" << endl;
    cout << "4 - email" << endl;
    cout << "5 - adres" << endl << endl;
    cout << "0 - powrot do menu" << endl;

    wybor = getch();

    switch(wybor){
        case '1':
            edytujAtrybutAdresata(index, id, adresy, adresy[index].imie);
            break;

        case '2':
            edytujAtrybutAdresata(index, id, adresy, adresy[index].nazwisko);
            break;

        case '3':
            edytujAtrybutAdresata(index, id, adresy, adresy[index].numerTelefonu);
            break;

        case '4':
            edytujAtrybutAdresata(index, id, adresy, adresy[index].email);
            break;

        case '5':
            edytujAtrybutAdresata(index, id, adresy, adresy[index].adres);
            break;

        case '0':
            ;
            break;

        default:
            ;
            break;
    }
}

void przejdzdoEdycjiAdresata(int idOstatniegoAdresata, vector<Adresat> &adresy){
    string wybraneIDJakoString = "";
    int wybraneID = 0;

    system("cls");
    cout << "Wpisz id adresata, ktorego chcesz edytowac: ";

    getline(cin, wybraneIDJakoString);
    wybraneID = atoi(wybraneIDJakoString.c_str());
    int index = wyszukajKontaktPoID(wybraneID, adresy);

    if(index >= 0 && index < idOstatniegoAdresata){
        edytujAdresata(index, wybraneID, adresy);
    }
    else{
        cout << "Nie znaleziono adresata o podanym id." << endl;
        Sleep(1500);
    }
}


void wypiszMenuLogowania(){
    system("cls");
	cout << "KSIAZKA ADRESOWA" << endl << endl;
	cout << "1. Logowanie" << endl;
	cout << "2. Rejestracja" << endl << endl;
	cout << "0. Zamknij program" << endl << endl;
}

void wypiszMenuGlowne(){
	system("cls");
	cout << "KSIAZKA ADRESOWA" << endl << endl;
	cout << "1. Dodaj adresata" << endl;
	cout << "2. Wyszukaj po imieniu" << endl;
	cout << "3. Wyszukaj po nazwisku" << endl;
	cout << "4. Wyswietl wszystkich adresatow" << endl;
	cout << "5. Usun adresata" << endl;
	cout << "6. Edytuj adresata" << endl << endl;
	cout << "8. Zmien haslo" << endl << endl;
	cout << "0. Wyloguj sie" << endl << endl;
}

int main(){
    int liczbaKontaktow = 0;
    int idOstatniegoAdresata = 0;
    char wybor = '0';

    Uzytkownicy uzytkownicy;
    vector <Adresat> adresaci;

    while(true){
        if(uzytkownicy.zwrocIdUzytkownika() == 0){
            wypiszMenuLogowania();
            wybor = getch();

            switch(wybor){
                case '1':
                    uzytkownicy.zaloguj();
                    liczbaKontaktow = wczytajAdresatowZPliku(adresaci, uzytkownicy.zwrocIdUzytkownika(), idOstatniegoAdresata);
                    break;

                case '2':
                    uzytkownicy.zarejestrujUzytkownika(nazwaPlikuZUzytkownikami);
                    break;

                case '0':
                    return 0;

                default:
                    cout << "Nie ma takiej opcji. Wybierz jeszcze raz." << endl;
                    Sleep(1500);
                    break;
            }

		}else{
            wypiszMenuGlowne();
            wybor = getch();

            switch(wybor){
			case '1':
				idOstatniegoAdresata = dodajKontakt(idOstatniegoAdresata, adresaci, uzytkownicy.zwrocIdUzytkownika());
				liczbaKontaktow++;
				break;

			case '2':
				wyszukajKontaktPoImieniu(adresaci);
				break;

            case '3':
				wyszukajKontaktPoNazwisku(adresaci);
				break;

			case '4':
				wypiszWszystkieKontakty(adresaci);
				break;

            case '5':
				przejdzDoUsuwaniaAdresata(idOstatniegoAdresata, adresaci);
				break;

            case '6':
				przejdzdoEdycjiAdresata(idOstatniegoAdresata, adresaci);
				break;

            case '8':
				uzytkownicy.zmienHaslo(nazwaPlikuZUzytkownikami);
				break;

			case '0':
				uzytkownicy.wyloguj();
				adresaci.clear();
                system("cls");
                cout << "Wylogowales sie.";
                Sleep(1500);
				break;

			default:
				cout << "Nie ma takiej opcji. Wybierz jeszcze raz." << endl;
				Sleep(1500);
				break;
            }
        }
    }
    return 0;
}
