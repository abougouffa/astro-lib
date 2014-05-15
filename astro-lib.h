/*
 * astro-lib.h
 * This file is part of AstroLib
 *
 * Copyright (C) 2011 - Abdelhak Mohammed Bougouffa [abdelhak.alg@gmail.com]
 *
 * AstroLib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * AstroLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


#ifndef __ASTRO_LIB_H__
#define __ASTRO_LIB_H__

#include <math.h>


typedef enum {
	Sunday		= 0,
	Monday		= 1,
	Tuesday		= 2,
	Wednesday	= 3,
	Thursday	= 4,
	Friday		= 5,
	Saturday	= 6
} WeekDay;


/* Structures */
typedef struct {
	int Year;
	int Month;
	float Day;
} Date;


/* Proto-types */
double JulianDayFromGregorian(int, int, float);
double JulianDayFromHijri(int, int, float);
double ModifiedJulianDayFromGregorian(int, int, float);
double JulianDay0FromGregorian(int);
double EquationOfTime(int, int, float);
WeekDay DayOfTheWeekFromGregorioan(int, int, float);
Date GregorianDate(double);
Date HijriDate(double, int);

#endif /* __ASTRO_LIB_H__ */
