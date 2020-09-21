#include "bmp.h"
#include <iostream>
#include <cstring>

BMP::BMP(unsigned int width, unsigned int height)
{
    pixels = new BMPTriple[width*height];
    bihPrepare(width, height);
    bfhPrepare();
    palPrepare();
    std::cout << sizeof(BMPTriple) << m_fileheader.offsetBits << '\n';
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

void BMP::write(char* arr)
{

}