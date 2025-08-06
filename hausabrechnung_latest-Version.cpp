/*
 * Hausabrechnung Minitool - alternative version
 * Contains similar functionality for distributing building costs.
 */
#include<string>
#include<vector>
#include<sstream>
#include<regex>
#include<iostream>
#include<stdio.h>
#include<sys/stat.h>
#include<fstream>
//#include<filesystem>

using namespace std;

string savepath;

unsigned int num_wohnung = 0;
vector<string> wohnung = {}; // name
vector<float> quadratmeter_wohnung = {}; // qm
vector<unsigned int> personen_wohnung = {}; // pn
vector<float> wasserzaehler = {}; // wr
vector<float> stromzaehler = {}; // st
vector<float> heizungszaehler = {}; // bilde Summe aus Heizungszaelerwerten
//vector<float> num_heizungen = {};

//kein Heizungsverbrauch nach Raum
//vector<unsigned int> wohnung_num_raum; // rn
//vector<string> raumname;
//vector<float> raum_heizungszaehler;
// skibatron miete

float gas_fixanteil = 0.3;
float gas_variabel = 0.7;

// Gesamtkosten
float strompreis; // sp
float wasser_gesamtkosten; // wrk
float wasser_gesamtverbrauch; // wrv
float gas_gesamtkosten; // gsk
float gas_verbrauchskosten; //

float gas_nebenkosten;
// fuer Gas Nebenkosten
//float gas_ablesung;
//float gas_wartungskosten;
//float gas_schornsteinfeger;
//float gas_abrechnungsdienst;


float gas_gesamtverbrauch; // gsv
float allgemeiner_stromverbrauch; // sak
float grundsteuer; // grk
float muellabfuhrkosten; // mrk
float wohngebaeudeversicherung; // wvk
float straßenreinigung; // sgk
float entwaesserungskosten; // ark
float allgemeine_stromkosten;

float wohnflaeche;
unsigned int personen_gesamt;
float heizung_gesamt;
float wasser_gesamt;

float wasserpreis;
float gaspreis;
float gaseinheitspreis;
float grundsteuer_m2;
float allgemeine_strom_person;
float muellabfuhrkosten_person;
float wohngebaeudeversicherung_m2;
float straßenreinigung_m2;
float entwaesserungskosten_m2;
float gasfixkostenpreis;

// Check string for valid format

    


bool checkStringA(const std::string& str) {

    std::regex pattern("sp=\\d+\\.\\d{1,4};wrk=\\d+\\.\\d{1,4};wrv=\\d+\\.\\d{1,4};gV=\\d+\\.\\d{1,4};gN=\\d+\\.\\d{1,4};gsv=\\d+\\.\\d{1,4};asv=\\d+\\.\\d{1,4};grk=\\d+\\.\\d{1,4};mrk=\\d+\\.\\d{1,4};wvk=\\d+\\.\\d{1,4};sgk=\\d+\\.\\d{1,4};ark=\\d+\\.\\d{1,4}");

    return std::regex_match(str, pattern);

}

bool checkStringB(const std::string& str) {

    std::regex pattern("name=\\w+;qm=\\d+(\\.\\d{1,4})?;pn=\\d+;wr=\\d+(\\.\\d{1,4})?;st=\\d+(\\.\\d{1,4})?;hz=\\d+(\\.\\d{1,4})?");

    return std::regex_match(str, pattern);

}


void save_data(const string& save_path) {

    FILE* sf = fopen(save_path.c_str(), "w");

   // Datei wurde geoeffnet, sf wird genutzt um in die Datei zu schreiben

    fprintf(sf,"sp=%.4f;wrk=%.4f;wrv=%.4f;gV=%.4f;gN=%.4f;gsv=%.4f;asv=%.4f;grk=%.4f;mrk=%.4f;wvk=%.4f;sgk=%.4f;ark=%.4f\n", strompreis, wasser_gesamtkosten, wasser_gesamtverbrauch, gas_verbrauchskosten, gas_nebenkosten ,gas_gesamtverbrauch, allgemeiner_stromverbrauch, grundsteuer, muellabfuhrkosten, wohngebaeudeversicherung, straßenreinigung, entwaesserungskosten);

    for (unsigned int i = 0; i < num_wohnung; i++) {
            fprintf(sf, "name=%s;qm=%.4f;pn=%u;wr=%.4f;st=%.4f;hz=%.4f\n", wohnung[i].c_str(), quadratmeter_wohnung[i], personen_wohnung[i], wasserzaehler[i], stromzaehler[i], heizungszaehler[i]); }

    fclose(sf);

}


void input_value(float& number, const std::string& message) {

    bool check = true;
    string eingabe;

    while (check) {

        //eingabe = "";

        try {
              std::cout << message;
              std::cin >> eingabe;
              //std::cout << endl;
              number = std::stof(eingabe);
              check = false;

        }
        catch (const std::invalid_argument& e) {
            std::cout << "Die Eingegebene Zeichenfolge ist keine gueltige Zahl ! Sie muessen den Wert nochmal korrekt angeben:\n";
            continue;
        }

    }

    return;

}

void print_general_values() {

    printf("\nWASSER\n");
    printf("Gesamtkosten: %.2f Euro\n", wasser_gesamtkosten);
    printf("Gesamtverbrauch: %.2f m3\n", wasser_gesamtverbrauch);
    printf("Preis pro Kubikmeter: %.2f Euro\n", wasser_gesamtkosten / wasser_gesamtverbrauch);
    printf("\nGAS\n");
    printf("GasVerbrauch Kosten: %.2f Euro\n", gas_verbrauchskosten);
    printf("Gesamtverbrauch: %.2f m3\n", gas_gesamtverbrauch);
    printf("Preis pro Kubikmeter: %.2f Euro\n", gas_gesamtkosten / gas_gesamtverbrauch);
    printf("Gas Nebenkosten: %.2f Euro\n", gas_nebenkosten);
    printf("Gas GESAMT-Kosten: %.2f Euro\n", gas_gesamtkosten);
    printf("\nSTROM (allgemein)\n");
    printf("allgemeiner Stromverbrauch: %.2f kWh\n", allgemeiner_stromverbrauch);
    printf("Strompreis: %.2f Euro/kWh\n", strompreis);
    printf("Allgemeine Stromkosten: %.2f Euro\n", allgemeiner_stromverbrauch * strompreis);
    printf("Stromkosten pro Person: %.2f Euro\n", allgemeine_strom_person);
    printf("\nWeitere Nebenkosten");
    printf("\nGrundsteuer (gesamt): %.2f Euro\n", grundsteuer);
    printf("Grundsteuer pro quadratmeter: %.2f Euro/m2\n", grundsteuer_m2);
    printf("Muellabfuhr (gesamt): %.2f Euro\n", muellabfuhrkosten);
    printf("Muellabfuhrkosten pro Person: %.2f Euro\n", muellabfuhrkosten_person);
    printf("Wohngebaeudeversicherung (gesamt): %.2f Euro\n", wohngebaeudeversicherung);
    printf("Wohngebaeudeversicherung pro Quadratmeter: %.2f Euro/m2\n", wohngebaeudeversicherung_m2);
    printf("Strassenreinigung (gesamt): %.2f Euro\n", straßenreinigung);
    printf("Strassenreinigung: %.2f Euro/m2\n", straßenreinigung_m2);
    printf("Entwaesserungskosten (gesamt): %.2f Euro\n", entwaesserungskosten);
    printf("Entwaesserungskosten: %.2f Euro/m2\n", entwaesserungskosten_m2);

}

void kennzahl_abfrage() {

    //input_value(hausflaeche, "Geben sie die Flaeche des gesamten Hauses in Quadratmeter m2 an. (Beispiel: 30 oder 27.5): ");
    //std::cout << "Hausflaeche in m2 = " << hausflaeche << "\n";

    float schmutzwasserkosten;
    float wasserrechnung;
    input_value(wasserrechnung, "Wie lauten die Gesamtkosten fuer den Wasserverbrauch (ohne Schmutzwassergebuehr) des Hauses in Euro (Beispiel: 189.99): ");
    input_value(schmutzwasserkosten, "Wie hoch ist die Schutzwassergebuehr vom gesamten Wasserverbrauch des Hauses in Euro: ");
    wasser_gesamtkosten = schmutzwasserkosten + wasserrechnung;
    std::cout << "Wasser (Gesamtkosten) in Euro = " << wasser_gesamtkosten << "\n";

    input_value(wasser_gesamtverbrauch, "Wie hoch ist der Gesamtverbrauch fuer Wasser des gesamten Hauses in Kubikmeter m3 (Beispiel: 40 oder 40.5): ");
    std::cout << "Wasser (Gesamtverbrauch) in m3 = " << wasser_gesamtverbrauch << "\n";
    
    input_value(gas_gesamtverbrauch, "Wie hoch ist der gesamte Gasverbrauch des Hauses in m3 (Kubikmeter), (Beispiel: 40 oder 40.5): ");
    std::cout << "Gas (Gesamtverbrauch) in m3 = " << gas_gesamtverbrauch << "\n";

    input_value(gas_verbrauchskosten, "Wie hoch sind die Gesamtkosten vom Gas-Verbrauch (ohne Nebenkosten) des gesamten Hauses in Euro (Beispiel 279.80): ");
    std::cout << "Gas (Verbrauchskosten) in Euro = " << gas_verbrauchskosten << "\n";
    
    std::cout << "ABFRAGE der Heizungs-Nebenkosten: " << endl;
    gas_nebenkosten = 0;
    float gas_ablesung;
    float gas_wartungskosten;
    float gas_schornsteinfeger;
    //float gas_abrechnungsdienst;
    input_value(gas_ablesung, "Wie hoch sind die Ablesekosten und/oder Miete fuer die Heizungszaehler in Euro: ");
    gas_nebenkosten += gas_ablesung;
    input_value(gas_wartungskosten,"Wie hoch sind die Wartungskosten fuer die Heizungen in Euro: ");
    gas_nebenkosten += gas_wartungskosten;
    input_value(gas_schornsteinfeger,"Wie hoch sind die Kosten fuer den Schornsteinfeger in Euro: ");
    gas_nebenkosten += gas_schornsteinfeger;
    std::cout << "Gas (Nebenkosten) in Euro = "<< gas_nebenkosten << endl;
    gas_gesamtkosten = gas_verbrauchskosten + gas_nebenkosten;
    std::cout << "Gas GESAMT-kosten in Euro = " << gas_gesamtkosten << endl;
    

    input_value(strompreis, "Wie hoch ist der Strompreis in Euro pro kW/h (Beispiel 6.99): ");
    std::cout << "Strompreis pro kW/h in Euro = " << strompreis << "\n";

    input_value(allgemeiner_stromverbrauch, "Wie hoch ist der allgemeine Stromverbrauch des Hauses in Kw/h (Beispiel: 25 oder 20.5): ");
    std::cout << "allgemeiner Stromverbrauch in kW/h = " << allgemeiner_stromverbrauch << "\n";

    input_value(grundsteuer, "Wie hoch sind die Grundsteuer fuer das ganze Haus (Grundstueck) in Euro (Beispiel 39.99): ");
    std::cout << "Grundsteuer in Euro = " << grundsteuer << "\n";

    input_value(muellabfuhrkosten, "Geben Sie die Gesamtkosten fuer die Muellabfuhr in Euro an (Beispiel 39.99): ");
    std::cout << "Muellabfuhrkosten in Euro = " << muellabfuhrkosten << "\n";

    input_value(wohngebaeudeversicherung, "Geben Sie die Gesamtkosten fuer die Wohngebaeudeversicherung in Euro an (Beispiel 39.99): ");
    std::cout << "Wohngebaeudeversicherung in Euro = " << wohngebaeudeversicherung << "\n";

    input_value(straßenreinigung, "Geben Sie die Gesamtkosten fuer die Strassenreinigung in Euro an (Beispiel 39.99): ");
    std::cout << "Strassenreinigung in Euro = " << straßenreinigung << "\n";

    input_value(entwaesserungskosten, "Geben Sie die Gesamtkosten fuer Entwaesserung in Euro an (Beispiel 39.99): ");
    std::cout << "Entwaesserungkosten in Euro = " << entwaesserungskosten << "\n";


}

void wohnung_aendern(unsigned int i) {

    if (i >= num_wohnung) {
        std::cout << "FEHLER: Ungueltige Eingabe. Die eingegebene Wohnungs-ID gehoert zu keiner existierenden Wohnung; Aktion abgebrochen" << endl;
        return;
    }

    char opt;
    std::cout << "Welcher Wert von Wohnung " << i << " von " << wohnung[i] << " soll geaendert werden ?" << endl;
    std::cout << "a: Name/Haushalt" << endl;
    std::cout << "b: Flaeche in m2 " << endl;
    std::cout << "c: Anzahl Personen" << endl;
    std::cout << "d: Stand des Wasserzaehlers" << endl;
    std::cout << "e: Stand des Stromzaehlers" << endl;
    std::cout << "f: Heizungen" << endl;
    std::cout << "g: ABBRUCH" << endl;
    std::cout << "Geben Sie den richtigen Buchstaben fuer den jeweiligen Wert an, den Sie aendern moechten: ";
    cin >> opt;

    string wohnungsname;
    string person_str;
    unsigned int personen;
    char confirm;
    bool check;
    float heizung;
    char next_room;
    string raumname;

    switch (opt) {

    case 'a':
    
           std::cout << "Geben Sie den Namen der Wohnung ein: ";
           cin >> wohnungsname;
           std::cout << endl;
           wohnung[i] = wohnungsname;
           break; 
    

    case 'b':
        input_value(quadratmeter_wohnung[i], "Geben Sie die Flaeche der Wohnung in m2 an: ");
        std::cout << endl;
        break;

    case 'c':
        check = true;
        while (check) {
            try {
                 
                std::cout << "Wie viele Personen wohnen im Raum ? (einfache Zahl): ";
                cin >> person_str;
                personen = std::stoul(person_str);
                check = false;

            }
            catch (const std::invalid_argument& e) {
                std::cout << "Die Eingegebene Zeichenfolge ist keine gueltige Zahl ! Sie muessen den Wert nochmal korrekt angeben:\n";
                continue;
            }
        }
        personen_wohnung[i] = personen;
        break;

    case 'd': // Stand des Wasserzaehlers
        input_value(wasserzaehler[i], "Geben Sie den Stand des Wasserzaehlers in m3 ein: ");
        std::cout << endl;
        break;
    case 'e': // Stromzaehler
        input_value(stromzaehler[i], "Geben Sie den Stand des Stromzaehlers in kW/h ein: ");
        std::cout << endl;
        break;
    case 'f':
        heizung = 0;
        std::cout << "Legen sie die Raeume der Wohnung neu an und geben den Zaehlerstand fuer jede Heizung der Wohnung ein.\n";
        next_room = 'n';

        do {
            
            std::cout << "Raum (Name): ";
            cin >> raumname;
            
            std::cout << "Geben Sie den Stand der ersten (oder einzigen) Heizung des Raumes an, weitere Heizungen koennen hinzugefuegt werden";
            float tmp;
            float hz = 0;
            input_value(tmp, "\nZaehlerstand der Heizung: ");
            hz += tmp;
            std::cout << "Heizung (Gesamteinheiten) von " << raumname << ": " << hz << endl;

            char next = 'n';
            std::cout << "Weiteren Heizungszaehler angeben ? (y/n): ";
            cin >> next;

            while (next == 'y') {
                input_value(tmp, "\nZaehlerstand der Heizung: ");
                hz += tmp;
                std::cout << "Heizung (Gesamteinheiten) von " << raumname << ": " << hz << endl;

                std::cout << "Weitere Heizung angeben ? (y/n): ";
                cin >> next;
            }

            heizung += hz;
            std::cout << "\nNaechsten Raum hinzufuegen ? (y/n): ";
            cin >> next_room;
        } while (next_room == 'y');
        heizungszaehler[i] = heizung;
        break;

    case 'g':
        std::cout << "Aktion abgebrochen" << endl;
        return;
    default:
        std::cout << "FEHLER: Ungueltige Option, Aktion wird abgebrochen" << endl;
        return;


    }


    std::cout << "Wohnung wurde geaendert -->" << endl;
    std::cout << "Name: " << wohnung[i] << endl;
    std::cout << "Flaeche: " << quadratmeter_wohnung[i] << endl;
    std::cout << "Bewohnerzahl: " << personen_wohnung[i] << endl;
    std::cout << "Strom: " << stromzaehler[i] << endl;
    std::cout << "Wasser: " << wasserzaehler[i] << endl;
    std::cout << "Heizung(Gas): " << heizungszaehler[i] << endl;

}

void wohnungsdaten_abfrage() {

    string wohnungsname;
    
    std::cout << "Geben Sie den Namen der Wohnung ein: ";
    std::cin >> wohnungsname;


    bool check = true;
    string person_str;
    unsigned int personen;
    while (check) {

        try {
            
            std::cout << "Wie viele Personen wohnen im Raum ? (einfache Zahl): ";
            cin >> person_str;
            personen = std::stoul(person_str);
            check = false;

        }
        catch (const std::invalid_argument& e) {
            std::cout << "Die Eingegebene Zeichenfolge ist keine gueltige Zahl ! Sie muessen den Wert nochmal korrekt angeben:\n";
            continue;
        }

    }

    float quadratmeter;
    input_value(quadratmeter, "Geben Sie die Flaeche der Wohnung in m2 an: ");
    //std::cout << endl;

    float wasserverbrauch;
    input_value(wasserverbrauch, "Geben Sie den Stand des Wasserzaehlers in m3 ein: ");
    //std::cout << endl;

    float stromverbrauch;
    input_value(stromverbrauch, "Geben Sie den Stand des Stromzaehlers in kW/h ein: ");
    std::cout << endl;


    float heizung = 0;
    std::cout << "Als naechstes legen sie die Raeume der Wohnung an und geben den Zaehlerstand fuer jede Heizung ein.\n";
    char next_room = 'n';

    do {
        string raumname;
        std::cout << "Raum (Name): ";
        cin >> raumname;

        std::cout << "Geben Sie den Stand der ersten (oder einzigen) Heizung des Raumes an, weitere Heizungen koennen hinzugefuegt werden";
        float tmp;
        float hz = 0;
        input_value(tmp, "\nZaehlerstand der Heizung: ");
        hz += tmp;
        std::cout << "Heizung (Gesamteinheiten) von " << raumname << ": " << hz << endl;

        char next = 'n';
        std::cout << "Weiteren Heizungszaehler angeben ? (y/n): ";
        cin >> next;

        while (next == 'y') {
            input_value(tmp, "\nZaehlerstand der Heizung: ");
            hz += tmp;
            std::cout << "Heizung (Gesamteinheiten) von " << raumname << ": " << hz << endl;

            std::cout << "Weitere Heizung angeben ? (y/n): ";
            cin >> next;
        }

        heizung += hz;
        std::cout << "\nNaechsten Raum hinzufuegen ? (y/n): ";
        cin >> next_room;


    } while (next_room == 'y');

    std::cout << "\nWohnung hinzugefuegt -->" << endl;
    std::cout << "Name: " << wohnungsname << endl;
    std::cout << "Flaeche: " << quadratmeter << endl;
    std::cout << "Bewohnerzahl: " << personen << endl;
    std::cout << "Strom: " << stromverbrauch << endl;
    std::cout << "Wasser: " << wasserverbrauch << endl;
    std::cout << "Heizung(Gas): " << heizung << endl;

    wohnung.push_back(wohnungsname);
    quadratmeter_wohnung.push_back(quadratmeter);
    personen_wohnung.push_back(personen);
    wasserzaehler.push_back(wasserverbrauch);
    stromzaehler.push_back(stromverbrauch);
    heizungszaehler.push_back(heizung);

    num_wohnung++;

}

int search_backwards(string target, char seperator) {

    for (unsigned int i = target.length() - 1; i >= 0; i--) {
        if (target[i] == seperator)
        {
            return i + 1;
        }
    }

    return -1;

}

int search_forward(string target, char seperator) {


    for (unsigned int i = 0; i < target.length(); i++) {
        if (target[i] == seperator)
        {
            return i + 1;
        }
    }

    return -1;

}

vector<string> splitString(string str, char delimiter)
{

    vector<string> stringList;

    stringstream ss(str);

    string token;
    while (std::getline(ss, token, delimiter))
    {
        stringList.push_back(token);
    }

    return stringList;
}

void load_main_data(string& dataline) {

    vector<string> kennzahlen = splitString(dataline, ';');

    //sp
    string token = kennzahlen[0];
    int vnidx = search_backwards(token, '=');
    //string namestring = token.substr(0,vnidx-1);
    string valstring = token.substr(vnidx);
    strompreis = std::stof(valstring);

    //wrk
    token = kennzahlen[1];
    vnidx = search_backwards(token, '=');
    //namestring = token.substr(0, vnidx - 1);
    valstring = token.substr(vnidx);
    wasser_gesamtkosten = std::stof(valstring);

    //wrv
    token = kennzahlen[2];
    vnidx = search_backwards(token, '=');
    //namestring = token.substr(0, vnidx - 1);
    valstring = token.substr(vnidx);
    wasser_gesamtverbrauch = std::stof(valstring);

    //gV
    token = kennzahlen[3];
    vnidx = search_backwards(token, '=');
    //namestring = token.substr(0, vnidx - 1);
    valstring = token.substr(vnidx);
    gas_verbrauchskosten = std::stof(valstring);
    
    //gN
    token = kennzahlen[4];
    vnidx = search_backwards(token, '=');
    //namestring = token.substr(0, vnidx - 1);
    valstring = token.substr(vnidx);
    gas_nebenkosten = std::stof(valstring);

    //gsv
    token = kennzahlen[5];
    vnidx = search_backwards(token, '=');
    //namestring = token.substr(0, vnidx - 1);
    valstring = token.substr(vnidx);
    gas_gesamtverbrauch = std::stof(valstring);

    //asv
    token = kennzahlen[6];
    vnidx = search_backwards(token, '=');
    //namestring = token.substr(0, vnidx - 1);
    valstring = token.substr(vnidx);
    allgemeiner_stromverbrauch = std::stof(valstring);

    //grk
    token = kennzahlen[7];
    vnidx = search_backwards(token, '=');
    //namestring = token.substr(0, vnidx - 1);
    valstring = token.substr(vnidx);
    grundsteuer = std::stof(valstring);

    //mrk
    token = kennzahlen[8];
    vnidx = search_backwards(token, '=');
    //namestring = token.substr(0, vnidx - 1);
    valstring = token.substr(vnidx);
    muellabfuhrkosten = std::stof(valstring);

    //wvk
    token = kennzahlen[9];
    vnidx = search_backwards(token, '=');
    //namestring = token.substr(0, vnidx - 1);
    valstring = token.substr(vnidx);
    wohngebaeudeversicherung = std::stof(valstring);

    //sgk
    token = kennzahlen[10];
    vnidx = search_backwards(token, '=');
    //namestring = token.substr(0, vnidx - 1);
    valstring = token.substr(vnidx);
    straßenreinigung = std::stof(valstring);

    //ark
    token = kennzahlen[11];
    vnidx = search_backwards(token, '=');
    //namestring = token.substr(0, vnidx - 1);
    valstring = token.substr(vnidx);
    entwaesserungskosten = std::stof(valstring);



}

void load_home_data(string& dataline) {

    //add an apartment
    num_wohnung += 1;

    //parse general_part

    vector<string> generalInfo = splitString(dataline, ';');

    string token = generalInfo[0];
    int vnidx = search_backwards(token, '=');
    string valstring = token.substr(vnidx);
    wohnung.push_back(valstring); // name

    token = generalInfo[1];
    vnidx = search_backwards(token, '=');
    valstring = token.substr(vnidx);
    float qm = std::stof(valstring);
    quadratmeter_wohnung.push_back(qm); // qm


    token = generalInfo[2];
    vnidx = search_backwards(token, '=');
    valstring = token.substr(vnidx);
    unsigned int pn = stoul(valstring);
    personen_wohnung.push_back(pn); // pn

    token = generalInfo[3];
    vnidx = search_backwards(token, '=');
    valstring = token.substr(vnidx);
    float wr = std::stof(valstring);
    wasserzaehler.push_back(wr); // wr

    token = generalInfo[4];
    vnidx = search_backwards(token, '=');
    valstring = token.substr(vnidx);
    float st = std::stof(valstring);
    stromzaehler.push_back(st); // st

    token = generalInfo[5];
    vnidx = search_backwards(token, '=');
    valstring = token.substr(vnidx);
    float hz = std::stof(valstring);
    heizungszaehler.push_back(hz); // hz

}

void change_single_value() {

    char val_ID;

    std::cout << "\nWelche Zahl moechten sie aendern ?" << endl;
    std::cout << "a: Wasser Gesamtkosten in Euro" << endl;
    std::cout << "b: Wasser Gesamtverbrauch in m3" << endl;
    std::cout << "c: Gas Gesamtverbrauch Kosten in Euro" << endl;
    std::cout << "d: Gas Gesamtverbrauch in m3" << endl;
    std::cout << "e: Strompreis pro kW/h in Euro" << endl;
    std::cout << "f: allgemeiner Stromverbrauch in kW/h" << endl;
    std::cout << "g: Grundsteuer in Euro" << endl;
    std::cout << "h: Muellabfuhrkosten in Euro" << endl;
    std::cout << "i: Wohngebaeudeversicherung (gesamt) in Euro" << endl;
    std::cout << "j: Strassenreinigung in Euro" << endl;
    std::cout << "k: Entwaesserungkosten in Euro" << endl;
    std::cout << "l: Gas Nebenkosten (Mehrfachabfrage)" << endl;
    std::cout << "m: Gas Fixkostenanteil (Voreinstellung = 0.3)" << endl;
    std::cout << "x: ABBRUCH" << endl;
    std::cout << "Geben Sie den richtigen Buchstaben fuer die jeweilige Kennzahl an, die Sie aendern moechten: ";
    cin >> val_ID;
    


    switch (val_ID) {
    case 'a':    
        float schmutzwasserkosten;
        float wasserrechnung;
        input_value(wasserrechnung, "Wie lauten die Gesamtkosten fuer den Wasserverbrauch (ohne Schmutzwassergebuehr) des Hauses in Euro (Beispiel: 189.99): ");
        input_value(schmutzwasserkosten, "Wie hoch ist die Schutzwassergebuehr vom gesamten Wasserverbrauch des Hauses in Euro: ");
        wasser_gesamtkosten = schmutzwasserkosten + wasserrechnung;
    std::cout << "Wasser (Gesamtkosten) in Euro = " << wasser_gesamtkosten << "\n";
        return;
    case 'b':
        input_value(wasser_gesamtverbrauch, "Wie hoch ist der Gesamtverbrauch fuer Wasser des gesamten Hauses in Kubikmeter m3 (Beispiel: 40 oder 40.5): ");
        std::cout << "Wasser (Gesamtverbrauch) in m3 = " << wasser_gesamtverbrauch << "\n";
        return;
    case 'c':
        input_value(gas_verbrauchskosten, "Wie hoch sind die Gesamtkosten fuer Gas des gesamten Hauses in Euro (Beispiel 279.80): ");
        gas_gesamtkosten = gas_verbrauchskosten + gas_nebenkosten;
        std::cout << "Gas (Gesamtkosten) in Euro = " << gas_gesamtkosten << endl;
        return;
    case 'd':
        input_value(gas_gesamtverbrauch, "Wie hoch ist der gesamte Gasverbrauch des Hauses in m3 (Kubikmeter), (Beispiel: 40 oder 40.5): ");
        std::cout << "Gas (Gesamtverbrauch) in m3 = " << gas_gesamtverbrauch << "\n";
        return;
    case 'e':
        input_value(strompreis, "Wie hoch ist der Strompreis in Euro pro kW/h (Beispiel 6.99): ");
        std::cout << "Strompreis pro kW/h in Euro = " << strompreis << "\n";
        return;
    case 'f':
        input_value(allgemeiner_stromverbrauch, "Wie hoch ist der allgemeine Stromverbrauch des Hauses in Kw/h (Beispiel: 25 oder 20.5): ");
        std::cout << "allgemeiner Stromverbrauch in kW/h = " << allgemeiner_stromverbrauch << "\n";
        return;
    case 'g':
        input_value(grundsteuer, "Wie hoch sind die Grundsteuer fuer das ganze Haus (Grundstück) in Euro (Beispiel 39.99): ");
        std::cout << "Grundsteuer in Euro = " << grundsteuer << "\n";
        return;
    case 'h':
        input_value(muellabfuhrkosten, "Geben Sie die Gesamtkosten fuer die Muellabfuhr in Euro an (Beispiel 39.99): ");
        std::cout << "Muellabfuhrkosten in Euro = " << muellabfuhrkosten << "\n";
        return;
    case 'i':
        input_value(wohngebaeudeversicherung, "Geben Sie die Gesamtkosten fuer die Wohngebaeudeversicherung in Euro an (Beispiel 39.99): ");
        std::cout << "Wohngebaeudeversicherung in Euro = " << wohngebaeudeversicherung << "\n";
        return;
    case 'j':
        input_value(straßenreinigung, "Geben Sie die Gesamtkosten fuer die Strassenreinigung in Euro an (Beispiel 39.99): ");
        std::cout << "Strassenreinigung in Euro = " << straßenreinigung << "\n";
        return;
    case 'k':
        input_value(entwaesserungskosten, "Geben Sie die Gesamtkosten fuer Entwaesserung in Euro an (Beispiel 39.99): ");
        std::cout << "Entwaesserungkosten in Euro = " << entwaesserungskosten << "\n";
        return;
    case 'l':
       std::cout << "ABFRAGE der Heizungs-Nebenkosten: " << endl;
       gas_nebenkosten = 0;
       float gas_ablesung;
       float gas_wartungskosten;
       float gas_schornsteinfeger;
       input_value(gas_ablesung, "Wie hoch sind die Ablesekosten und/oder Miete fuer die Heizungszaehler in Euro: ");
       gas_nebenkosten += gas_ablesung;
       input_value(gas_wartungskosten,"Wie hoch sind die Wartungskosten fuer die Heizungen in Euro: ");
       gas_nebenkosten += gas_wartungskosten;
       input_value(gas_schornsteinfeger,"Wie hoch sind die Kosten fuer den Schornsteinfeger in Euro: ");
       gas_nebenkosten += gas_schornsteinfeger;
       std::cout << "Gas (Nebenkosten) in Euro = "<< gas_nebenkosten << endl;
       gas_gesamtkosten = gas_verbrauchskosten + gas_nebenkosten;
       std::cout << "Gas GESAMT-kosten in Euro = " << gas_gesamtkosten << endl;
       return;            
    case 'm':
            input_value(gas_fixanteil,"Geben Sie den Anteil der Gaskosten an der fix nach Quadratmeter abgerechnet wird in Prozent (Beispiel 30): ");
            gas_fixanteil = gas_fixanteil/100;
            gas_variabel = 1 - gas_fixanteil;
    case 'x':
        return;
    default:
        std::cout << "Ungueltige Eingabe --> Abbruch, bitte geben sie naechstes mal einen Buchstaben zwischen a-l an." << endl;
        return;

    }

}

bool compute_measures() {

    if (num_wohnung <= 0) {
        std::cout << "FEHLER: Die Aktion kann nicht durchgefuehrt werden\nSie haben keine Wohnungen angelegt. Fuegen Sie mit der jeweiligen Option mindestens eine Wohnung hinzu" << endl;
        return false;
    }

    wohnflaeche = 0;
    personen_gesamt = 0;
    heizung_gesamt = 0;
    wasser_gesamt = 0;
    gas_gesamtkosten = gas_verbrauchskosten + gas_nebenkosten;

    for (unsigned int i = 0; i < num_wohnung; i++) {
        wohnflaeche += quadratmeter_wohnung[i];
        personen_gesamt += personen_wohnung[i];
        heizung_gesamt += heizungszaehler[i];
        wasser_gesamt += wasserzaehler[i];
    }

    if (wohnflaeche < 1) {
        std::cout << "FEHLER: Die Aktion kann nicht durchgefuehrt werden\nSie haben falsche Angben zur Flaeche der Wohnungen in m2 gemacht. Diese müssen zusammen mindestens = 1 ergeben !" << endl;
        std::cout << "Loeschen Sie alle Wohnungen und legen Sie alles neu an oder bearbeiten Sie eine bestimmte Wohnung um die Flaeche zu aendern." << endl;
        return false;
    }

    if (personen_gesamt < 1) {
        std::cout << "FEHLER: Die Aktion kann nicht durchgefuehrt werden\nSie haben keiner Wohnung Personen hinzugefuegt. Loeschen Sie alle Wohnungen und legen Sie alles neu an oder bearbeiten Sie eine bestimmte Wohnung um eine Personenzahl von mindestens 1 anzugeben" << endl;
        return false;
    }
/*
    if (heizung_gesamt < 1) {
        std::cout << "FEHLER: Die Aktion kann nicht durchgefuehrt werden\nSie haben keiner Wohnung Heizungszaehler hinzugefuegt. Loeschen Sie alle Wohnungen und legen Sie alles neu an oder bearbeiten Sie eine bestimmte Wohnung um einen korrekten Heizungswert anzugeben" << endl;
        return false;
    }

*/
    //allgemeine Stromkosten
    allgemeine_stromkosten = allgemeiner_stromverbrauch * strompreis;

    //wasserpreis
    if (wasser_gesamtverbrauch < 1) {
        std::cout << "FEHLER: Die Aktion kann nicht durchgefuehrt werden\nSie haben keinen Wert groesser 1 für den Wasser Gesamtverbrauch angegeben. Aendern Sie die Kennzahl mit der jeweiligen Option" << endl;
        return false;
    }
    wasserpreis = wasser_gesamtkosten / wasser_gesamt;

    /* gaspreis
    if (gas_verbrauchskosten < 1) {
        std::cout << "FEHLER: Die Aktion kann nicht durchgefuehrt werden\nSie haben keinen Wert groesser 1 für den Gas Gesamtverbrauch angegeben. Aendern Sie die Kennzahl mit der jeweiligen Option" << endl;
        return false;
    }
    
    */
    gaspreis = gas_verbrauchskosten / gas_gesamtverbrauch;
    
    heizung_gesamt = std::max(heizung_gesamt,float(0.1));
    gaseinheitspreis = (gas_gesamtkosten*gas_variabel)/heizung_gesamt;
    gasfixkostenpreis = (gas_gesamtkosten*gas_fixanteil)/wohnflaeche;

    //allgemeinstrom_person
    allgemeine_strom_person = allgemeine_stromkosten / personen_gesamt;

    //grundsteuer_m2
    grundsteuer_m2 = grundsteuer / wohnflaeche;

    //wohngebaeudeversicherung_m2
    wohngebaeudeversicherung_m2 = wohngebaeudeversicherung / wohnflaeche;

    //muellabfuhrkosten_person
    muellabfuhrkosten_person = muellabfuhrkosten / personen_gesamt;

    //straßenreinigung_m2
    straßenreinigung_m2 = straßenreinigung / wohnflaeche;

    //entwaesserungskosten_m2
    entwaesserungskosten_m2 = entwaesserungskosten / wohnflaeche;

    return true;


}

void wohnungen_anzeigen() {

    unsigned int ID = 0;

    unsigned int colsize = 13; // mindestgroesse für die Spalten der Tabelle
    //unsigned int wlen = 0;
    int rowsize = 14;
    string subline = "--------------|"; // colsize +1 +1
    string row_line = "--------------|"; // rowsize +1

    //first row head
    std::cout << "\n WOHNUNG:     "; // OK, rowsize 
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*s  ", colsize, wohnung[w].c_str()); } // colsize + 2 seperation
    std::cout << endl;

    // Unterteilung 
    std::cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { std::cout << subline; }
    std::cout << endl;

    //second row, ID
    std::cout << " ID           |";// rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) {
        printf("%*u  |", colsize - 1, ID);
        ID++;
    }
    std::cout << endl;

    // Unterteilung 
    std::cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { std::cout << subline; }
    std::cout << endl;

    //third row flaeche(m2)
    std::cout << " Flaeche (m2) |"; // rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*.2f  |", colsize - 1, quadratmeter_wohnung[w]); }
    std::cout << endl;

    // Unterteilung 
    std::cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { std::cout << subline; }
    std::cout << endl;


    //fourth row, personen
    std::cout << " Personen     |";
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*u  |", colsize - 1, personen_wohnung[w]); }
    std::cout << endl;

    // Unterteilung 
    std::cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { std::cout << subline; }
    std::cout << endl;

    //fifth row, stromzaehler
    std::cout << " Strom (kW/h) |";// rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*.2f  |", colsize - 1, stromzaehler[w]); }
    std::cout << endl;

    // Unterteilung 
    std::cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { std::cout << subline; }
    std::cout << endl;

    //sixth row, wasserzaehler
    std::cout << " Wasser (m3)  |";// rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*.2f  |", colsize - 1, wasserzaehler[w]); }
    std::cout << endl;

    // Unterteilung 
    std::cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { std::cout << subline; }
    std::cout << endl;

    //seventh row, heizungszaehler
    std::cout << " Heizung      |"; // rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*.2f  |", colsize - 1, heizungszaehler[w]); }
    std::cout << endl;

    // Unterteilung 
    std::cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { std::cout << subline; }
    std::cout << endl;


}

void hausabrechnung() {

    //Maßstab
    std::cout << "\nRechnungsschluessel und Masstab\n" << endl;
    printf("Wasserpreis: %.2f Euro/m3\n", wasser_gesamtkosten / wasser_gesamt);
    printf("Gaspreis: %.2f Euro/m3\n", gas_verbrauchskosten / gas_gesamtverbrauch);
    printf("allgemeine_Stromkosten: %.2f Euro\n", allgemeine_strom_person);
    printf("Grundsteuer: %.2f Euro/m2\n", grundsteuer_m2);
    printf("Muellabfuhrkosten(pro Person): %.2f Euro\n", muellabfuhrkosten_person);
    printf("Wohngebaeudeversicherung: %.2f Euro/m2\n", wohngebaeudeversicherung_m2);
    printf("Strassenreinigung: %.2f Euro/m2\n", straßenreinigung_m2);
    printf("Entwaesserungkosten: %.2f Euro/m2\n", entwaesserungskosten_m2);

    unsigned int colsize = 13; // mindestgroesse für die Spalten der Tabelle
    //unsigned int wlen = 0;
    int rowsize = 27;
    string subline = "--------------|"; // colsize +1 +1
    string row_line = "---------------------------|"; // rowsize +1
    vector<float> summen = {};
    for (unsigned int w = 0; w < num_wohnung; w++) { summen.push_back(0); }

    //first row head
    std::cout << "\n\n WOHNUNG:                  "; // OK, rowsize 
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*s  ", colsize, wohnung[w].c_str()); } // colsize + 2 seperation
    std::cout << endl;

    // Unterteilung 
    std::cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { std::cout << subline; }
    std::cout << "-------" << endl;

    // stromkosten
    std::cout << " Stromkosten (Euro)        |"; // rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { 
        printf("%*.2f  |", colsize - 1, stromzaehler[w] * strompreis);
        summen[w] += stromzaehler[w] * strompreis;
    }
    std::cout << "  EUR" << endl;

    // Unterteilung 
    std::cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { std::cout << subline; }
    std::cout << "-------" << endl;

    //second
    std::cout << " Wasserkosten (Euro)       |"; // rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { 
        printf("%*.2f  |", colsize - 1, wasserzaehler[w] * wasserpreis);
        summen[w] += wasserzaehler[w] * wasserpreis;
    }
    std::cout << "  EUR" << endl;

    // Unterteilung 
    std::cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { std::cout << subline; }
    std::cout << "-------" << endl;

    //third	
    std::cout << " Heizungskosten (gesamt)   |";
    for (unsigned int w = 0; w < num_wohnung; w++) { 
        printf("%*.2f  |", colsize - 1, (heizungszaehler[w]*gaseinheitspreis)+(quadratmeter_wohnung[w]*gasfixkostenpreis) );
        summen[w] += (heizungszaehler[w]*gaseinheitspreis)+(quadratmeter_wohnung[w]*gasfixkostenpreis);
    }
    std::cout << "  EUR" << endl;

    // Unterteilung 
    std::cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { std::cout << subline; }
    std::cout << "-------" << endl;

    //third 
    std::cout << " allgemeine Stromkosten    |"; // rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { 
        printf("%*.2f  |", colsize - 1, personen_wohnung[w] * allgemeine_strom_person);
        summen[w] += personen_wohnung[w] * allgemeine_strom_person;
    }
    std::cout << "  EUR" << endl;

    // Unterteilung 
    std::cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { std::cout << subline; }
    std::cout << "-------" << endl;

    //
    std::cout << " Grundsteuer (Euro)        |";// rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { 
        printf("%*.2f  |", colsize - 1, quadratmeter_wohnung[w] * grundsteuer_m2);
        summen[w] += quadratmeter_wohnung[w] * grundsteuer_m2;
    }
    std::cout << "  EUR" << endl;

    // Unterteilung 
    std::cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { std::cout << subline; }
    std::cout << "-------" << endl;

    //seventh
    std::cout << " Wohngebaeudeversicherung  |"; // rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { 
        printf("%*.2f  |", colsize - 1, quadratmeter_wohnung[w] * wohngebaeudeversicherung_m2);
        summen[w] += quadratmeter_wohnung[w] * wohngebaeudeversicherung_m2;
    }
    std::cout << "  EUR" << endl;

    // Unterteilung 
    std::cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { std::cout << subline; }
    std::cout << "-------" << endl;

    std::cout << " Muellabfuhrkosten (Euro)  |"; // rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { 
        printf("%*.2f  |", colsize - 1, personen_wohnung[w] * muellabfuhrkosten_person);
        summen[w] += personen_wohnung[w] * muellabfuhrkosten_person;
    }
    std::cout << "  EUR" << endl;

    // Unterteilung 
    std::cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { std::cout << subline; }
    std::cout << "-------" << endl;

    std::cout << " Strassenreinigung (Euro)  |"; // rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { 
        printf("%*.2f  |", colsize - 1, quadratmeter_wohnung[w] * straßenreinigung_m2);
        summen[w] += quadratmeter_wohnung[w] * straßenreinigung_m2;
    }
    std::cout << "  EUR" << endl;

    // Unterteilung 
    std::cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { std::cout << subline; }
    std::cout << "-------" << endl;

    std::cout << " Entwaesserung (Euro)      |"; // rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { 
        printf("%*.2f  |", colsize - 1, quadratmeter_wohnung[w] * entwaesserungskosten_m2); 
        summen[w] += quadratmeter_wohnung[w] * entwaesserungskosten_m2;
    }
    std::cout << "  EUR" << endl;

    // Unterteilung 
    std::cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { std::cout << subline; }
    std::cout << "-------" << endl;

    std::cout << "          SUMME            |";
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*.2f  |", colsize - 1, summen[w]); }
    std::cout << "  EUR\n" << endl;

    /*for (unsigned int w = 0; w < num_wohnung; w++) {
        wlen = wohnung[w].size();
        if (wlen > longest) { longest = wlen; }
    }*/

}

bool main_dialog() {

    char option;
    std::cout << "\nHAUPTMENUE\nWaehlen Sie eine der folgenden Optionen aus -\n";
    std::cout << "a: HAUSABRECHNUNG durchfueren und anzeigen\n";
    std::cout << "b: Eingaben und Daten speichern\n";
    std::cout << "c: Neue Wohnung anlegen\n";
    std::cout << "d: Anzeigen gespeicherter Kennzahlen\n";
    std::cout << "e: Anzeigen gespeicherter Wohnungsdaten\n";
    std::cout << "f: Neue Kennzahlen eingeben\n";
    std::cout << "g: Einzelne Kennzahl aendern\n";
    std::cout << "h: Wohnungsdaten loeschen\n";
    std::cout << "i: Daten einzelner Wohnung aendern\n";
    std::cout << "j: ENDE\n";
    std::cout << "Geben Sie den Buchstaben der gewuenschten Option ein: ";
    cin >> option;

    string ID_str;

    switch (option) {

    case 'a':
        if (compute_measures()) { hausabrechnung(); }
        return true;

    case 'b':
        std::cout << "\nSpeichere Eingabedaten in Datei..." << endl;
        if (compute_measures()) { save_data("abrechnung.sav"); }
        return true;

    case 'c':
        std::cout << "\nNEUE WOHNUNG ANLEGEN , Daten werden abgefragt... " << endl;
        wohnungsdaten_abfrage();
        return true;

    case 'd':
        if (compute_measures()) { print_general_values(); }
        return true;

    case 'e':
        if (num_wohnung >= 1) { wohnungen_anzeigen(); }
        else { std::cout << "Es sind keine Wohnungen eingegeben oder gespeichert. Legen Sie zuerst Wohnungen mit der jeweiligen Option an." << endl; }
        return true;

    case 'f':
        kennzahl_abfrage();
        return true;

    case 'g':
        change_single_value();
        return true;

    case 'h':
        std::cout << "Alle Wohnungen wurden geloescht und muessen neu eingegeben werden" << endl;
        num_wohnung = 0;
        wohnung = {}; // name
        quadratmeter_wohnung = {}; // qm
        personen_wohnung = {}; // pn
        wasserzaehler = {}; // wr
        stromzaehler = {}; // st
        heizungszaehler = {};
        return true;

    case 'i':
        if (num_wohnung <= 0) {
            cout << "FEHLER: Es sind momentan keine Wohnungen registriert. Sie muessen erst welche mit der jeweiligen Option anlegen.\nAktion abgebrochen" << endl;
            return true;
        }
        std::cout << "\nVerfuegbare Wohnungen:\n" << endl;
        wohnungen_anzeigen();
        unsigned int ID;
        
        try {
            std::cout << "\nGeben Sie die jeweilige ID der Wohnung ein, dessen Werte veraendert werden sollen: ";
            cin >> ID_str;
            ID = std::stoul(ID_str);
            wohnung_aendern(ID);
        }
        catch (const std::invalid_argument& e) { std::cout << "Der eingegebene Wert ist keine gueltige ID als ganze Zahl. Fuehren Sie die Aktion mit korrekter Eingabe erneut durch !" << endl; }
        
        return true;

    case 'j':
        // ENDE
        return false;
    default:
        std::cout << "FEHLER, bitte geben Sie einen Buchstaben ein, der fuer eine gueltige Option steht !"; //<< endl;
        return true;
        //break;

    }

}



int main(int argc, char* argv[]) {

    //num_wohnung = 0;

    std::cout << "HAUSABRECHNUNG MINITOOL 1.0" << endl;
    std::cout << "\nWillkommen beim Programm fuer Ihre Hausabrechnung -\nSuche nach gespeicherten Daten..." << endl;

    /* //überflüssig

    string loadpath = argv[0];
    unsigned int sep_position = 0;
    for (int i = loadpath.length() - 1; i > 1; i--) {
        if (loadpath[i] == '\\' || loadpath[i] == '/')
        {
            sep_position = i + 1;
            break;
        }
    }
    string resulting_string = loadpath.substr(0, sep_position);
    savepath = resulting_string + "abrechnung.sav";
    //std::cout << "resulting string is " << newpath << "\n"; */

    ifstream ifile;
    ifile.open("abrechnung.sav");
    if (ifile) { // Speicherdatei existiert => Lade Daten in das Programm

        string first_line;
        std::cout << "Speicherdatei vorhanden, wird geladen..." << endl;
        if (getline(ifile, first_line)) {
            if (checkStringA(first_line)) {
                load_main_data(first_line);

                string line;
                while (getline(ifile, line)) {
                    if (checkStringB(line)) { load_home_data(line); }
                    else { std::cout << "FEHLER beim Lesen der Wohnungsdaten, eine Wohnung wird nicht hinzugefuegt"; }
                }

            }
            else {
                std::cout << "FEHLER: Die Speicherdatei ist nicht korrekt formatiert, beschaedigt oder manipuliert.\nAlle Daten muessen leider erneut eingegeben werden\n" << endl;
                kennzahl_abfrage();
                printf("\nWASSER\n");
                printf("Gesamtkosten: %.2f Euro\n", wasser_gesamtkosten);
                printf("Gesamtverbrauch: %.2f m3\n", wasser_gesamtverbrauch);
                printf("Preis pro Kubikmeter: %.2f Euro\n", wasser_gesamtkosten / wasser_gesamtverbrauch);
                printf("\nGAS\n");
                printf("Gesamtkosten: %.2f Euro\n", gas_gesamtkosten);
                printf("Gesamtverbrauch: %.2f m3\n", gas_gesamtverbrauch);
                printf("Preis pro Kubikmeter: %.2f Euro\n", gas_gesamtkosten / gas_gesamtverbrauch);
                printf("\nSTROM (allgemein)\n");
                printf("allgemeiner Stromverbrauch: %.2f kWh\n", allgemeiner_stromverbrauch);
                printf("Strompreis: %.2f Euro/kWh\n", strompreis);
                printf("Allgemeine Stromkosten: %.2f Euro\n", allgemeiner_stromverbrauch * strompreis);
                printf("\nWeitere Nebenkosten\n");
                printf("\nGrundsteuer (gesamt): %.2f Euro\n", grundsteuer);
                printf("Muellabfuhr (gesamt): %.2f Euro\n", muellabfuhrkosten);
                //printf("Muellabfuhrkosten pro Person: %.2f Euro\n", muellabfuhrkosten_person);
                printf("Wohngebaeudeversicherung (gesamt): %.2f Euro\n", wohngebaeudeversicherung);
                printf("Strassenreinigung (gesamt): %.2f Euro\n", straßenreinigung);
                printf("Entwaesserungkosten (gesamt): %.2f Euro\n", entwaesserungskosten);
                std::cout << "\nAls naechstes muessen sie mindestens eine Wohnung anlegen, bitte geben Sie die gewuenschten Daten ein >>" << endl;

                char nxt = 'n';
                do {
                    wohnungsdaten_abfrage();
                    std::cout << "\nWEITERE WOHNUNG ANGEBEN ? (y/n): ";
                    cin >> nxt;
                } while (nxt == 'y');

            }
        }
        else {
            std::cout << "FEHLER: Die Speicherdatei ist nicht korrekt formatiert, beschaedigt oder manipuliert.\nAlle Daten muessen leider erneut eingegeben werden\n" << endl;
            kennzahl_abfrage();
            printf("\nWASSER\n");
            printf("Gesamtkosten: %.2f Euro\n", wasser_gesamtkosten);
            printf("Gesamtverbrauch: %.2f m3\n", wasser_gesamtverbrauch);
            printf("Preis pro Kubikmeter: %.2f Euro\n", wasser_gesamtkosten / wasser_gesamtverbrauch);
            printf("\nGAS\n");
            printf("Gesamtkosten: %.2f Euro\n", gas_gesamtkosten);
            printf("Gesamtverbrauch: %.2f m3\n", gas_gesamtverbrauch);
            printf("Preis pro Kubikmeter: %.2f Euro\n", gas_gesamtkosten / gas_gesamtverbrauch);
            printf("\nSTROM (allgemein)\n");
            printf("allgemeiner Stromverbrauch: %.2f kWh\n", allgemeiner_stromverbrauch);
            printf("Strompreis: %.2f Euro/kWh\n", strompreis);
            printf("Allgemeine Stromkosten: %.2f Euro\n", allgemeiner_stromverbrauch * strompreis);
            printf("\nWeitere Nebenkosten\n");
            printf("\nGrundsteuer (gesamt): %.2f Euro\n", grundsteuer);
            printf("Muellabfuhr (gesamt): %.2f Euro\n", muellabfuhrkosten);
            //printf("Muellabfuhrkosten pro Person: %.2f Euro\n", muellabfuhrkosten_person);
            printf("Wohngebaeudeversicherung (gesamt): %.2f Euro\n", wohngebaeudeversicherung);
            printf("Strassenreinigung (gesamt): %.2f Euro\n", straßenreinigung);
            printf("Entwaesserungkosten (gesamt): %.2f Euro\n", entwaesserungskosten);
            std::cout << "\nAls naechstes muessen sie mindestens eine Wohnung anlegen, bitte geben Sie die gewuenschten Daten ein >>" << endl;
            char nxt = 'n';
            do {
                wohnungsdaten_abfrage();
                std::cout << "\nWEITERE WOHNUNG ANGEBEN ? (y/n): ";
                cin >> nxt;
            } while (nxt == 'y');
        }

        ifile.close();

    }

    else {

        std::cout << "Speicherdatei nicht gefunden oder umbennant, starte mit Abfrage der Abrechnungsdaten... \n" << endl;
        kennzahl_abfrage();
        printf("\nWASSER\n");
        printf("Gesamtkosten: %.2f Euro\n", wasser_gesamtkosten);
        printf("Gesamtverbrauch: %.2f m3\n", wasser_gesamtverbrauch);
        printf("Preis pro Kubikmeter: %.2f Euro\n", wasser_gesamtkosten / wasser_gesamtverbrauch);
        printf("\nGAS\n");
        printf("Gesamtkosten: %.2f Euro\n", gas_gesamtkosten);
        printf("Gesamtverbrauch: %.2f m3\n", gas_gesamtverbrauch);
        printf("Preis pro Kubikmeter: %.2f Euro\n", gas_gesamtkosten / gas_gesamtverbrauch);
        printf("\nSTROM (allgemein)\n");
        printf("allgemeiner Stromverbrauch: %.2f kWh\n", allgemeiner_stromverbrauch);
        printf("Strompreis: %.2f Euro/kWh\n", strompreis);
        printf("Allgemeine Stromkosten: %.2f Euro\n", allgemeiner_stromverbrauch * strompreis);
        printf("\nWeitere Nebenkosten\n");
        printf("\nGrundsteuer (gesamt): %.2f Euro\n", grundsteuer);
        printf("Muellabfuhr (gesamt): %.2f Euro\n", muellabfuhrkosten);
        //printf("Muellabfuhrkosten pro Person: %.2f Euro\n", muellabfuhrkosten_person);
        printf("Wohngebaeudeversicherung (gesamt): %.2f Euro\n", wohngebaeudeversicherung);
        printf("Strassenreinigung (gesamt): %.2f Euro\n", straßenreinigung);
        printf("Entwaesserungkosten (gesamt): %.2f Euro\n", entwaesserungskosten);
        std::cout << "\nAls naechstes muessen sie mindestens eine Wohnung anlegen, bitte geben Sie die gewuenschten Daten ein >>" << endl;
        char nxt = 'n';
        do {
            wohnungsdaten_abfrage();
            std::cout << "\nWEITERE WOHNUNG ANGEBEN ? (y/n): ";
            cin >> nxt;
        } while (nxt == 'y');

    }

    ifile.close();

    bool status = true;
    while (status) { status = main_dialog(); }



    /* char save_dialog = 'y';
    std::cout << "Werte speichern ? (y/n): ";
    cin >> save_dialog;
    if (save_dialog == 'y') { save_values(); } */

    //cin.get();
    return 0;

}


