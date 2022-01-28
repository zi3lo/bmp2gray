#include "bmp_io.h"

Bmp_io::Bmp_io(const char * fin) //konstruktor (1 argument)
{
    file_in = fin;
    file_out = "\0";
}

Bmp_io::Bmp_io(const char * fin, const char * fout) //konstruktor (2 argumenty)
{
    file_in = fin;
    file_out = fout;
}

Bmp_io::~Bmp_io() //destruktor, sprzątamy
{
    bmp_in.close();
    bmp_out.close();
    delete [] pixmap;
}

void Bmp_io::err(const char *s) //wyswietla blad
{
    perror(s);
    exit(1);
}

void Bmp_io::open_in() //otwieramy plik źródłowy
{
    bmp_in.open(file_in, std::ios::binary);
    if (!bmp_in) err("Nie mozna otworzyc pliku zrodlowego!\n");
}

void Bmp_io::open_out() //otwieramy plik wynikowy
{
    bmp_out.open(file_out, std::ios::binary);
    if (!bmp_out)
    {
        bmp_out.close();
        err("Nie mozna otworzyc pliku wyjsciowego!\n");
    }
}

void Bmp_io::read_in() //odczyt danych z pliku źródłowego
{
    bmp_in.read((char *)head,54);
    width = *(int*)&head[18];
    height = *(int*)&head[22];
    data_start = *(int*)&head[10]; //poczatek tabeli pixeli
    add = (width*3)%4 ? 4-(width*3)%4 : 0; // wyrównanie wiersza do wielokrotności 4
    newwidth = width+add;
    pixmap = new BGR[height*newwidth]; //tworzymy tablicę pixeli
    bmp_in.seekg(data_start);
    for (i = 0; i < height; i++) // pętla czytająca mapę pixeli
    {
        for (j = 0; j < width; j++)
        {
            bmp_in.read((char*)&pixmap[width*i+j].B,1);
            bmp_in.read((char*)&pixmap[width*i+j].G,1);
            bmp_in.read((char*)&pixmap[width*i+j].R,1);
        }
        bmp_in.seekg(add,std::ios::cur);
    }
    bmp_in.clear();
    bmp_in.seekg(0,std::ios::beg); //powrót do początku pliku
}

void Bmp_io::write_out() //zapis pliku wynikowego wraz z konwersją do skali szarości
{
    for (i=0; i<data_start; i++) //kopiujemy nagłówek aż do danych
    {
        bmp_in.read((char*)&ch,1);
        bmp_out.write((char*)&ch,1);
    }

    for (i=0; i<width*height; i++) //zamieniamy na skalę szarości
    {
        szary = (uint8_t)((pixmap[i].B*0.114 + pixmap[i+1].G*0.587 + pixmap[i+2].R*0.229)); //wartości ze względu na specyfikę oka
        pixmap[i].B=pixmap[i].G=pixmap[i].R=szary;
    }
    for (i = 0; i < height; i++) // pętla zapisująca nową mapę pixeli
    {
        for (j = 0; j < width; j++)
        {
            bmp_out.write((char*)&pixmap[width*i+j].B,1);
            bmp_out.write((char*)&pixmap[width*i+j].G,1);
            bmp_out.write((char*)&pixmap[width*i+j].R,1);
        }
        for (int z = 0; z < add; z++)
            bmp_out.write("\0",1);

    }
}

void Bmp_io::in_fo()
{
    std::cout << "Szerokosc obrazu: " << width << std::endl;
    std::cout << "Wysokosc obrazu: " << height << std::endl;
    std::cout << "Dane zaczynaja sie od: " << data_start << " bajtu" << std::endl;
    std::cout << "Dopelnienie do '4': " << add << std::endl;
}

void Bmp_io::go() //odpalamy procedurę otwierania i zmiany parametrów
{
    open_in();
    open_out();
    read_in();
    std::cout << "Parametry pliku zrodlowego: \n";
    in_fo();
    write_out();
}
