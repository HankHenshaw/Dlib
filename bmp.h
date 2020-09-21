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
    BitmapFileHeader m_fileheader;
    BitmapInfoHeader m_infoheader;
    char pallete[1024];
    BMPTriple *pixels;

    void bfhPrepare();
    void bihPrepare(unsigned int width, unsigned int height);
    void palPrepare();
public:
    BMP(unsigned int width, unsigned int height);
    void write(char* arr);
};