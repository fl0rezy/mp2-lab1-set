// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len <= 0) {
        throw "Error";
    }
    BitLen = len;
    MemLen = (BitLen + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen) {
        throw "Error";
    }
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen) {
        throw "Error";
    }
    int bit_pos;
    bit_pos = n % (sizeof(TELEM) * 8);
    return TELEM(1) << bit_pos;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) {
        throw "Error";
    }
    int bit_index, mask;
    bit_index = GetMemIndex(n);
    mask = GetMemMask(n);
    pMem[bit_index] = pMem[bit_index] | mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) {
        throw "Error";
    }
    int bit_index, mask;
    bit_index = GetMemIndex(n);
    mask = GetMemMask(n);
    pMem[bit_index] = pMem[bit_index] & ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) {
        throw "Error";
    }
    int bit_index, mask;
    bit_index = GetMemIndex(n);
    mask = GetMemMask(n);
    if ((pMem[bit_index] & mask) > 0) {
        return 1;
    }
    return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this == &bf) {
        return *this;
    }
    delete[] pMem;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    int max_len = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    for (int i = 0; i < max_len; i++) {
        int bit1 = (i < BitLen) ? GetBit(i) : 0;
        int bit2 = (i < bf.BitLen) ? bf.GetBit(i) : 0;
        if (bit1 != bit2)
            return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf)// операция "или"
{
    int max_len = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField res(max_len);

    for (int i = 0; i < BitLen; i++) {
        if (GetBit(i)) {
            res.SetBit(i);
        }
    }
    for (int i = 0; i < bf.BitLen; i++) {
        if (bf.GetBit(i)) {
            res.SetBit(i);
        }
    }

    return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int min_len = (BitLen < bf.BitLen) ? BitLen : bf.BitLen;
    TBitField res(min_len);

    for (int i = 0; i < min_len; i++) {
        if (GetBit(i) && bf.GetBit(i)) {
            res.SetBit(i);
        }
    }
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);

    for (int i = 0; i < BitLen; i++) {
        if (!GetBit(i)) {
            res.SetBit(i);
        }
    }

    return res;
}

// ввод/вывод


istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    char ch;
    int i = 0;
    while (istr.get(ch) && ch != ' ' && ch != '\n' && i < bf.GetLength()) {
        if (ch == '1') {
            bf.SetBit(i);
        }
        else if (ch == '0') {
            bf.ClrBit(i);
        }
        i++;
    }

    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
