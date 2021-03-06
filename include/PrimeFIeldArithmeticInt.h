﻿/***********************************************************************\
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
* \file PrimeFieldArithmetic.h
* Header file for prime field arithmetic functions.
*
* Present library is designed to allow deploying on 8, 16, 32 and 64 bit
* platforms. User have to define globals in "Globals.h" accordingly.
*
* First steps to get a working environment:
* 1) Define a pfproperties object, it will be unique for operating on a
* specific field.
* 2) Call InitFieldProperties() to set parameters and allocate needed memory.
* 3) Define some pfelement that will represent elements belonging to the
* previously defined field.
* 4) Call InitElement() to set element values and allocate memory.
* 5) Now perform operations among field elements.
* 6) Call FreeElement() on every element to free memory.
* 7) Call FreeFieldProperties() to free field structure memory.
*
* Warning: no null pointer deferencing check is performed on current
* library functions. It is responsibility of the developer to provide valid
* data to present code.
*
* References for following algorithms:
* [1] D. Hankerson, S. Vanstone, and A.J. Menezes. Guide to Elliptic Curve
* Cryptography. Springer Professional Computing. Springer, 2004.
* [2] W. Hasenplaugh, G. Gaubatz, V. Gopal. Fast Modular Reduction.
* Proceedings of ARITH'07, 2007.
* [3] A.J. Menezes, P.C. van Oorschot, S.A. Vanstone. Handbook of Applied
* Cryptography. Taylor & Francis, 2010.
* [4] D.E. Knuth, The Art of Computer Programming – Seminumerical Algorithms,
* volume 2, Addison-Wesley, Reading, Massachusetts, 2nd edition, 1981.
* [5] C. Koc, High-speed RSA implementation, Technical Report, RSA
* Laboratories, 1994.
* [6] K. Geddes, S. Czapor, and G. Labahn, Algorithms for Computer Algebra,
* Kluwer Academic Publishers, Boston, 1992.
*
*
* Notes:
* From [3] some notes on best algorithms for multiprecision arithmetic are
* available. In general for modulo reduction, Montgomery reduction and barrett
* reduction are comparable. Concerning multiplication, the Karatsuba-Ofman
* algorithm is considered a valid solution to improve efficiency of the
* classical multiplication. For a more detailed treatment of the Karatsuba-
* Ofman algorithm, see Knuth [4], Koc [5], and Geddes, Czapor, and Labahn
* [6]. A detailed treatment of methods for performing modular multiplication
* can be found in Knuth [4].
*
* The analysis carried in [1] defines a list of algorithms:
* - Addition in Fp (algorithm 2.7)
* - Subtraction in Fp (algorithm 2.8)
* - Integer multiplication, product scanning form (algorithm 2.10)(+)
* - Karatsuba-Ofman multiplication (example 2.12)
* - Barrett reduction (algorithm 2.14)(+)
* - Fast reduction algorithms for NIST primes (algorithms 2.27-2.31)(+)
* - Binary algorithm for inversion in Fp (algorithm 2.22)(+)
* - Partial Montgomery inversion in Fp (algorithm 2.23)
* - Montgomery inversion in Fp (algorithm 2.25, including calls to 2.23)
*
* Some of above algorithms are included in the sofware implementation timing
* analysis described in [1], chapter 5. In such chapter, (+) show best
* results.
*/

#ifndef CPRIMEFIELFARITHMETICINT_H
#define CPRIMEFIELFARITHMETICINT_H

#include "../include/MultiprecisionArithmeticInt.h"

//! Prime field element type
typedef struct _mpnumber pfelement;

//! Field properties structure
struct _pfproperties
{
	unsigned int bits;			//!< Field dimension in bits
	unsigned int chunksNumber;	//!< Number of chunks of every field element
	mpnumber characteristics;	//!< Field characteristics
	mpnumber mu;				//!< Barrett reduction precomputed term
};

//! Field properties type
typedef struct _pfproperties pfproperties;

//! Checks if a is equal to b
//! \returns 1 if a is equal to b, 0 otherwise
unsigned int PFEquals(pfelement * a, pfelement * b, pfproperties * field);

//! Checks if a is greater or equal than b.
//! \returns 1 if a is greater than or equal to b, 0 otherwise
unsigned int GreaterOrEqual(
	pfelement * a,
	pfelement * b,
	pfproperties * field);

//! Sets the specified bit size and characteristics in the field object
//! \param field The field to fill (must point to already allocated memory)
//! \param fieldBitSize The field bis size
//! \param characteristics Hex string describing the field characteristics
void InitFieldProperties(
	pfproperties * field,
	unsigned int fieldBitSize,
	char * characteristics);

//! Sets the specified value expressed via a hex string in the field element 
//! object. Needs the presence of a pfproperties object storing field bitsize, 
//! chunks number and characteristics.
//! \param element The target element to write
//! \param hexString The string value to write
//! \param field The object storing field data
void InitElementByString(
	pfelement * element, 
	char * hexString,
	pfproperties * field);

//! Erase the memory of internal members of element
void FreeElement(pfelement * element);

//! Erase the memory of internal members of field
void FreeFieldProperties(pfproperties * field);

//! Addition of a and b over a prime field. Let p the field characteristics, 
//! the method evaluates a+b(mod p)
//! Algorithm 2.7 [1]
void PFAddition(
	pfelement * sum,
	pfelement * a,
	pfelement * b,
	pfproperties * field);

//! Subtraction of b from a over a prime field. Let p the field 
//! characteristics, the method evaluates a-b(mod p)
//! Algorithm 2.8 [1]
void PFSubtraction(
	pfelement * sub,
	pfelement * a,
	pfelement * b,
	pfproperties * field);

//! Multiplication of a and b over a prime field. Let p the field 
//! characteristics, the method evaluates ab(mod p)
void PFMultiplication(
	pfelement * mul,
	pfelement * a,
	pfelement * b,
	pfproperties * field);

//! Division of a and b over a prime field. Let p the field 
//! characteristics, the method evaluates a/b(mod p)
void PFDivision(
	pfelement * div,
	pfelement * a,
	pfelement * b,
	pfproperties * field);

//! General modulo reduction
//! Algorithm 2.14 [1]
//! http://cacr.uwaterloo.ca/hac/about/chap14.pdf
void BarrettReduction(pfelement * red, mpnumber * a, pfproperties * field);

//! Modified Barrett modulo reduction [2]
void ModifiedBarretReduction(
	pfelement * red,
	mpnumber * a,
	pfproperties * field);

//! Fast modulo reduction for p = 2^192 − 2^64 − 1 (FIPS 186-4 prime)
//! Algorithm 2.27 [1]
void FastReductionFIPSp192(
	pfelement * red,
	pfelement * a,
	pfproperties * field);

//! Fast modulo reduction for p = 2^224 − 2^96 + 1 (FIPS 186-4 prime)
//! Algorithm 2.28 [1]
void FastReductionFIPSp224(
	pfelement * red,
	pfelement * a,
	pfproperties * field);

//! Fast modulo reduction for p = 2^256 - 2^224 + 2^192 + 2^96 - 1 
//! (FIPS 186-4 prime)
//! Algorithm 2.29 [1]
void FastReductionFIPSp256(
	pfelement * red,
	pfelement * a,
	pfproperties * field);

//! Fast modulo reduction for p = 2^384 - 2^128 - 2^96 + 2^32 - 1 
//! (FIPS 186-4 prime)
//! Algorithm 2.30 [1]
void FastReductionFIPSp384(
	pfelement * red,
	pfelement * a,
	pfproperties * field);

//! Fast modulo reduction for p = 2^521 - 1 (FIPS 186-4 prime)
//! Algorithm 2.31 [1]
void FastReductionFIPSp521(
	pfelement * red,
	pfelement * a,
	pfproperties * field);

#endif // CPRIMEFIELFARITHMETICINT_H
