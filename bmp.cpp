#include "bmp.h"
#include <iostream>
#include <cstring>

BMP::BMP(unsigned int width, unsigned int height)
{
    m_size = width*height;
    pixels = new BMPTriple[m_size];
    bihPrepare(width, height);
    bfhPrepare();
    palPrepare();
    std::cout << sizeof(BMPTriple) << m_fileheader.offsetBits << '\n';
}

BMP::~BMP()
{
    delete pixels;
}

void BMP::bfhPrepare()
{
    m_fileheader.type = 0x4D42; // 'BM'
    /*
        bfh.bfSize = bfh.bfOffBits +
         sizeof(color) * Width * Height +
         Height * ((sizeof(color) * Width) % 4);
    */
    m_fileheader.reserved1 = 0;
    m_fileheader.reserved2 = 0;
    m_fileheader.offsetBits = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + 1022; // - 2, т.к. размер структуры из-за выравнивания = 16
    m_fileheader.size = m_fileheader.offsetBits + 3 * m_infoheader.width * m_infoheader.height + m_infoheader.height * (m_infoheader.width % 4); 
}

void BMP::bihPrepare(unsigned int width, unsigned int height)
{
    m_infoheader.length = sizeof(BitmapInfoHeader);
    m_infoheader.width = static_cast<int>(width);
    m_infoheader.height = static_cast<int>(height);
    m_infoheader.planes = 1; // Кол-во плоскостей
    m_infoheader.bitCount = 24;
    m_infoheader.compression = 0; //BI_RGB
    m_infoheader.sizeImage = 0; // т.к. изображение не сжато
    m_infoheader.xPelsPerMeter = 0; // TODO: Надоли менять?
    m_infoheader.yPelsPerMeter = 0; // TODO: Надоли менять?
    m_infoheader.colorsUsed = 16777216; // 2^24
    m_infoheader.colorsImportant = 0;
}

void BMP::palPrepare()
{
    std::memset(pallete, 0, sizeof(pallete));
}

void BMP::addPixel(unsigned int x, unsigned int y, unsigned char red, unsigned char green, unsigned char blue)
{
    BMPTriple elem{red, green, blue};
    pixels[x*m_infoheader.width+y] = elem; // TODO: Проверить правильность расположения(т.е. х и у)
}

std::ostream& operator<<(std::ostream &out, const BMP& obj)
{   //TODO: Доступ к полям через геттеры?
    out << obj.m_fileheader.type
        << obj.m_fileheader.size
        << obj.m_fileheader.reserved1
        << obj.m_fileheader.reserved2
        << obj.m_fileheader.offsetBits;

    out << obj.m_infoheader.length
        << obj.m_infoheader.width
        << obj.m_infoheader.height
        << obj.m_infoheader.planes
        << obj.m_infoheader.bitCount
        << obj.m_infoheader.compression
        << obj.m_infoheader.sizeImage
        << obj.m_infoheader.xPelsPerMeter
        << obj.m_infoheader.yPelsPerMeter
        << obj.m_infoheader.colorsUsed
        << obj.m_infoheader.colorsImportant;

    for(size_t i = 0; i < 1024; ++i)
    {
        out << obj.pallete[i];
    }

    for(size_t i = 0; i < obj.m_size; ++i)
    {
        out << obj.pixels[i].blue
            << obj.pixels[i].green
            << obj.pixels[i].red;
        //TODO: Нужно ли добивать нулями до 4-ёх байт?
    }
    //TODO: Нужно ли добивать файл нулями что бы он был кратен 4-ем байтам?
    return out;
}