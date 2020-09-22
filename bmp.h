#pragma once

//TODO: данный бмп будет работать только для 32-бит
struct BMPTriple
{
    char red;
    char green;
    char blue;
};

struct BitmapFileHeader
{
    short type;
    int size;
    short reserved1;
    short reserved2;
    int offsetBits; 
};

struct BitmapInfoHeader
{
    int length;
    int width;
    int height;
    short planes;
    short bitCount;
    int compression;
    int sizeImage;
    int xPelsPerMeter;
    int yPelsPerMeter;
    int colorsUsed;
    int colorsImportant;
};

class BMP
{
public: //TODO: Remove!
    BitmapFileHeader m_fileheader;
    BitmapInfoHeader m_infoheader;
    char pallete[1024];
    BMPTriple *pixels; //TODO: Сделать двухмерным
    unsigned int m_size;

    void bfhPrepare();
    void bihPrepare(unsigned int width, unsigned int height);
    void palPrepare();
public:
    BMP(unsigned int width, unsigned int height);
    void addPixel(unsigned int x, unsigned int y, char red, char green, char blue);
    friend std::ofstream& operator<<(std::ofstream &fout, const BMP& obj);
};