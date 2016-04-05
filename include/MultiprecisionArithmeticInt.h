/***********************************************************************\
* ecclib - Efficient library for elliptic curve cryptography            *
* Copyright (C) 2015  Alessio Leoncini                                  *
*                                                                       *
* This program is free software: you can redistribute it and/or modify  *
* it under the terms of the GNU General Public License as published by  *
* the Free Software Foundation, either version 3 of the License, or     *
* (at your option) any later version.                                   *
*                                                                       *
* This program is distributed in the hope that it will be useful,       *
* but WITHOUT ANY WARRANTY; without even the implied warranty of        *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
* GNU General Public License for more details.                          *
*                                                                       *
* You should have received a copy of the GNU General Public License     *
* along with this program.  If not, see <http://www.gnu.org/licenses/>. *
\***********************************************************************/

/*!
* \file MultiprecisionArithmetic.h
* Header file for multiple precision positive integer arithmetic functions.
*/

#ifndef MULTIPRECISIONARITHMETICINT_H
#define MULTIPRECISIONARITHMETICINT_H

#include "Globals.h"

//! Generic multiple precision number structure
struct _mpnumber
{
	chunk * data;
	unsigned int size;
};

//! Generic multiple precision number type
typedef struct _mpnumber mpnumber;

//! Writes the hexdecimal value from hexString to element, strictly following 
//! the specified bitsize
//! \param hexString The hexdecimal value to write (MSB letf)
//! \param chunksNumber The number of chunks already allocated
//! \param bitSize The size in bits of the underlying field
//! \param number The multiple precision number to fill
void SetString(
	mpnumber * number,
	char * hexString,
	unsigned int chunksNumber,
	unsigned int bitSize);

//! Give a string containing the hexadecimal representation of the number.
//! This is not an efficient implementation, use only for debug purposes.
//! \param chunksNumber The number of chunks of given prime field element
//! \param bitSize The size in bits of prime field element
//! \param element The prime field element
//! \returns The hexadecimal representation string, remember to free such
//! memory
char * GetString(
	unsigned int chunksNumber,
	unsigned int bitSize,
	mpnumber * number);

//! Sets the specified value expressed via a hex string in the number.
//! \param number The target element to write
//! \param hexString The string value to write
//! \param bitSize Size in bits of number
void InitNumberByString(
	mpnumber * number,
	char * hexString,
	unsigned int bitSize);

//! Initialize memory of specified number.
//! \param number The target element to write
//! \param chunksNumber Chunks number
void InitNumber(mpnumber * number, unsigned int chunksNumber);

//! Erase the memory of internal members of number
void FreeNumber(mpnumber * number);

//! Multiple precision addition algorithm
//! 
void MPAddition(mpnumber * sum, mpnumber * a, mpnumber * b);

//! Long division algorithm [4], computing the quotient of a/b with the 
//! corresponding remainder. 
//! \param div The quotient
//! \param rem The remainder
//! \param u The dividend
//! \param v The divisor
void LongDivision(mpnumber * div, mpnumber * rem, mpnumber * u, mpnumber * v);

//! Short division algorithm [4], computing the quotient of a/b with the 
//! corresponding remainder, with b as a single precision number.
//! \param div The quotient
//! \param rem The remainder
//! \param a The dividend
//! \param b The divisor
void ShortDivision(mpnumber * div, mpnumber * rem, mpnumber * a, mpnumber * b);

#endif // MULTIPRECISIONARITHMETICINT_H