/***********************************************************************\
* CPrimeFieldArithmetic - Prime Field Arithmetic primitives             *
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

#include "../include/CPrimeFieldArithmeticInt.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

int main()
{
	wprintf(L"CPrimeFieldArithmetic fast modular reduction test");

	field_t field;
	SetField(&field, 192, "fffffffffffffffffffffffffffffffeffffffffffffffff");
	element_t a;
	unsigned int cnumber = 384 / ARCHITECTURE_BITS;
	if (384 % ARCHITECTURE_BITS)
	{
		cnumber++;
	}
	a.data = (chunk_t *)malloc(cnumber * sizeof(chunk_t));
	SetString("f32825fc7b46e221f33861bad98abfaa7a919b3169eb3fc87f43e82788d59eb78f09ed1b06e4fcc5bb85e55acf4", cnumber, 384, a.data);
	char * hexdump = GetString(cnumber, 384, a.data);
	a.data[0];
	a.data[1];
	a.data[2];
	a.data[10];

	element_t reduced;
	SetElement(&reduced, "", &field);
	FastReductionFIPSp192(&reduced, &a, &field);

	char * hexdump2 = GetString(field.chunksNumber, field.bits, reduced.data);

	reduced.data[0];
	reduced.data[1];
	reduced.data[2];
	reduced.data[3];
	reduced.data[4];
	reduced.data[5];

	element_t true_red;
	SetElement(&true_red, "221eb65a46fdedda74084effc1003c59fbc71293fa688b47", &field);

	if (Equals(&reduced, &true_red, &field))
	{
		return 1;
	}
	return 0;
}
