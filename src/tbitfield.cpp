// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len) : BitLen(len)
{
    if (len < 1)
        throw 0;
    MemLen = (len + 31) >> 5;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) 
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) 
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n >> 5;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n & 31);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n > -1 && n < BitLen)
        pMem[GetMemIndex(n)] |= GetMemMask(n);
    else if (n < 0)
        throw - 1;
    else throw 1;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n > -1 && n < BitLen)
        pMem[GetMemIndex(n)] &= ~GetMemMask(n);
    else if (n < 0)
        throw - 1;
    else throw 1;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n > -1 && n < BitLen)
        return pMem[GetMemIndex(n)] & GetMemMask(n);
    else if (n < 0)
        throw - 1;
    else throw 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) return *this;
    BitLen = bf.BitLen;
    if (MemLen != bf.MemLen)
    {
        MemLen = bf.MemLen;
        if (pMem != nullptr) delete pMem;
        pMem = new TELEM[MemLen];
    }
    for (int i = 0; i < MemLen; i++) 
        pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 0;
    else
        for (int i = 0; i < MemLen; i++)
            if (pMem[i] != bf.pMem[i])
                return 0;
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (this == &bf)
        return 0;
    return 1;
}

TBitField TBitField::operator| (const TBitField &bf) // операция "или"
{
    int len = BitLen;
    if (bf.BitLen > len) len = bf.BitLen;
    TBitField temp(len);
    for (int i = 0; i < MemLen; i++) 
        temp.pMem[i] = pMem[i];
    for (int i = 0; i < bf.MemLen; i++) 
        temp.pMem[i] |= bf.pMem[i];
    return temp;
}

TBitField TBitField::operator& (const TBitField &bf) // операция "и"
{
    int len = BitLen;
    if (bf.BitLen > len) len = bf.BitLen;
    TBitField temp(len);
    for (int i = 0; i < MemLen; i++) temp.pMem[i] = pMem[i];
    for (int i = 0; i < bf.MemLen; i++) temp.pMem[i] &= bf.pMem[i];
    return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; i++)
        result.pMem[i] = ~pMem[i];
    TELEM temp = (1 << (BitLen & 31)) - 1;
    result.pMem[MemLen - 1] &= temp;
    return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int i(0); char ch;
    do { istr >> ch; } while (ch != ' ');
    while (true)
    {
        istr >> ch;
        if (ch == '0') bf.ClrBit(i++);
        else if (ch == '1') bf.SetBit(i++);
        else break;
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    int len = bf.GetLength();
    for (int i = 0; i < len; i++)
        if (bf.GetBit(i)) ostr << '1'; else ostr << '0';
    return ostr;
}
