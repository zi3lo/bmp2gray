#include <iostream>
#include <fstream>
#include "bmp_io.h"

using namespace std;

struct BGR
{
    uint8_t B,G,R;
};

void show_menu();

int main (int argc, char** argv)
{

    if (argc == 2) //w zależności od ilości parametrów odpowiednia akcja (info o pliku bmp / konwersja na odcienie szarości / wyświeltenie info)
    {
       Bmp_io file(argv[1]);
       file.open_in();
       file.read_in();
       file.in_fo();
    }
    else if (argc == 3)
    {
        Bmp_io file(argv[1],argv[2]);
        file.go();
    }
    else
    {
        show_menu();
    }

    return 0;
}

void show_menu() //funkcja pokazująca 'menu'
{
    cout << "\nProgram do zamiany plików bmp na odcienie szaerości\n\n"
         << "Skladnia: \n"
         << "bmp2gray [zrodlo]  -wyświeltenie info o pliku BMP\n"
         << "bmp2gray [zrodlo] [cel]  -konwersja na odcienie szarości\n";
}
