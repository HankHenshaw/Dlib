#include "bmp.h"
#include <iostream>
#include <cstring>

BMP::BMP(unsigned int width, unsigned int height)
{
    m_size = width*height;
    pixels = new BMPTriple[m_size];
    for(size_t i = 0; i < m_size; ++i)
    {
        pixels[i] = {255, 255, 255};
    }
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
    m_infoheader.xPelsPerMeter = 0;
    m_infoheader.yPelsPerMeter = 0; 
    m_infoheader.colorsUsed = 16777216; // 2^24
    m_infoheader.colorsImportant = 0;
}

void BMP::palPrepare()
{
    std::memset(pallete, 0, sizeof(pallete));
}

void BMP::addPixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue)
{
    if(x < 0 || y < 0)
        return;
    if(m_infoheader.width < x || m_infoheader.height < y) 
        return;
    BMPTriple elem{red, green, blue};
    pixels[(m_infoheader.width-y-1)*m_infoheader.width+x] = elem;
}

std::ostream& operator<<(std::ostream &out, BMP& obj)
{
    out.write(reinterpret_cast<char*>(&obj.getFileHeader().type), 2);
    out.write(reinterpret_cast<char*>(&obj.getFileHeader().size), 4);
    out.write(reinterpret_cast<char*>(&obj.getFileHeader().reserved1), 2);
    out.write(reinterpret_cast<char*>(&obj.getFileHeader().reserved2), 2);
    out.write(reinterpret_cast<char*>(&obj.getFileHeader().offsetBits), 4);

    out.write(reinterpret_cast<char*>(&obj.getInfoHeader().length), 4);
    out.write(reinterpret_cast<char*>(&obj.getInfoHeader().width), 4);
    out.write(reinterpret_cast<char*>(&obj.getInfoHeader().height), 4);
    out.write(reinterpret_cast<char*>(&obj.getInfoHeader().planes), 2);
    out.write(reinterpret_cast<char*>(&obj.getInfoHeader().bitCount), 2);
    out.write(reinterpret_cast<char*>(&obj.getInfoHeader().compression), 4);
    out.write(reinterpret_cast<char*>(&obj.getInfoHeader().sizeImage), 4);
    out.write(reinterpret_cast<char*>(&obj.getInfoHeader().xPelsPerMeter), 4);
    out.write(reinterpret_cast<char*>(&obj.getInfoHeader().yPelsPerMeter), 4);
    out.write(reinterpret_cast<char*>(&obj.getInfoHeader().colorsUsed), 4);
    out.write(reinterpret_cast<char*>(&obj.getInfoHeader().colorsImportant), 4);

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

BitmapFileHeader& BMP::getFileHeader()
{
    return m_fileheader;
}

BitmapInfoHeader& BMP::getInfoHeader()
{
    return m_infoheader;
}