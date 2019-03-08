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

void podzielString(string linia, char znak, vector<string> &wyrazy){

    for(int i=0, j=0, dlugosc=linia.length(); j<=dlugosc; j++){
        if(linia[j] == znak){
            wyrazy.push_back(linia.substr(i,j-i));
            i = j+1;
        }
    }
}

int wczytajUzytkownikowZPliku(vector<Uzytkownik> &uzytkownicy){
    Uzytkownik uzytkownik;
    int liczbaUzytkownikow = 0;
    string linia, temp;
	fstream plik;
	vector<string> podzielonaLinia;

	plik.open(nazwaPlikuZUzytkownikami.c_str(), ios::in);
    if(plik.good()==false){
        cout << "Nie udalo sie otworzyc pliku Uzytkownicy.txt. Zostanie utworzony nowy plik.";
        Sleep(1500);
        plik.open(nazwaPlikuZUzytkownikami.c_str(), ios::out | ios::app);
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

    return liczbaUzytkownikow;
}

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

int zaloguj(vector <Uzytkownik> &uzytkownicy, int &liczbaUzytkownikow){
    string login, haslo;

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

                    cout << "Zalogowales sie.";
                    Sleep(1000);
                    return uzytkownicy[i].zwrocID();
                }
                cout << "Podane haslo jest nieprawidlowe.";
                Sleep(2000);
            }
            cout << endl << "Podales 3 razy bledne haslo.";
            Sleep(1500);
            return 0;
        }
        i++;
    }
    cout << "Brak w bazie uzytkownika o podanym loginie.";
    Sleep(1500);
    return 0;
}

void zarejestrujUzytkownika(vector <Uzytkownik> &uzytkownicy, int &liczbaUzytkownikow){
    fstream plik;
    int id;
    string login, haslo;

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

    plik.open(nazwaPlikuZUzytkownikami.c_str(), ios::out | ios::app);
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

void zmienHaslo(vector <Uzytkownik> &uzytkownicy, int liczbaUzytkownikow, int idUzytkownika){
    string haslo;
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

    plik.open(nazwaPlikuZUzytkownikami.c_str(), ios::trunc | ios::out);
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
    int idUzytkownika = 0;
    int liczbaKontaktow = 0;
    int idOstatniegoAdresata = 0;
    char wybor = '0';

    vector <Uzytkownik> uzytkownicy;
    vector <Adresat> adresaci;

    int liczbaUzytkownikow = wczytajUzytkownikowZPliku(uzytkownicy);

    while(true){
        if(idUzytkownika == 0){
            wypiszMenuLogowania();
            wybor = getch();

            switch(wybor){
                case '1':
                    idUzytkownika = zaloguj(uzytkownicy, liczbaUzytkownikow);
                    liczbaKontaktow = wczytajAdresatowZPliku(adresaci, idUzytkownika, idOstatniegoAdresata);
                    break;

                case '2':
                    zarejestrujUzytkownika(uzytkownicy, liczbaUzytkownikow);
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
				idOstatniegoAdresata = dodajKontakt(idOstatniegoAdresata, adresaci, idUzytkownika);
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
				zmienHaslo(uzytkownicy, liczbaUzytkownikow, idUzytkownika);
				break;

			case '0':
				idUzytkownika = 0;
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
