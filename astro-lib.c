/*
 * astro-lib.c
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



#include <stdio.h>
#include "astro-lib.h"
#include <math.h>
#include <string.h>


/*
 * JD: The Julian Day is a countinuous count days and fractions
 * thereof from the begining of the year -4712.
 * NOTE: This method valid for positive and negative years, but not for negative Julian Days.
 */
double JulianDayFromGregorian(int year, int month, float day)
{
	char is_julian_calendar = 0;
	char is_gregorian_calendar = 0;

	int a, b;

	if (month == 1 || month == 2) {
		year--;
		month += 12;
	}

	if (year < 1582)				is_julian_calendar = 1;
	else if (year > 1582)			is_gregorian_calendar = 1;
	else if (year == 1582) {
		if (month < 10)				is_julian_calendar = 1;
		else if (month > 10)		is_gregorian_calendar = 1;
		else if (month == 10) {
			if (day < 15)			is_julian_calendar = 1;
			else if (day >= 15)		is_gregorian_calendar = 1;
		}
	}

	if (is_julian_calendar)			b = 1;
	else if (is_gregorian_calendar)	a = (int)(year / 100); b = 2 - a + (int)(a / 4);

	return ((int)(365.25 * (year + 4716)) + (int)(30.6001 * (month + 1)) + day + b - 1524.5);
}


/*
 * Get the Julian Day from Hijri (Islamic) date.
 */
double JulianDayFromHijri(int year, int month, float day)
{
	int alpha	= (int)((11 * year + 3) / 30), 
	beta		= (int)(354 * year),
	gamma		= (int)(30 * month) - (int)((month - 1) / 2);

	return alpha + beta + gamma + day + 1948440 - 385;
}


Date GregorianDate(double jd)
{
	int alpha, a, b, c, d, e, z = (int)jd;
	float f = jd - z;
	Date date;
	
	if (z < 2299161) a = z;
	else {
		alpha = (int)((z - 1867216.25) / 36524.25);
		a = z + 1 + alpha - (int)(alpha / 4);
	}
	
	b = a + 1524;
	c = (int)((b - 122.1) / 365.25);
	d = (int)(365.25 * c);
	e = (int)((b - d) / 30.6001);
	
	/* Calculate the day */
	date.Day = b - d - (int)(30.6001 * e) + f;
	
	/* Calculate the month */
	if (e < 14) date.Month = e - 1;
	else if (e == 14 || e == 15) date.Month = e - 13;
	
	/* Calculate the year */
	if (date.Month > 2) date.Year = c - 4716;
	else if (date.Month == 1 || date.Month == 2) date.Year = c - 4715;
	
	return date;
}


Date HijriDate(double jd, int correction_val)
{
	int alpha, beta, lambda = (int)(jd + correction_val) - 1937808;
	Date date;
	
	alpha = (int)((lambda - 1) / 10631);
	lambda = lambda - 10631 * alpha + 354;
	beta = (int)((10985 - lambda) / 5316)
         * (int)((50 * lambda) / 17719)
         + (int)(lambda / 5670)
         * (int)((43 * lambda) / 15238);
	lambda = lambda - (int)((30 - beta) / 15)
                    * (int)((17719 * beta) / 50)
                    - (int)(beta / 16)
                    * (int)((15238 * beta) / 43) + 29;
	
	date.Month = (int)((24 * lambda) / 709);
	date.Day = lambda - (int)((709 * date.Month) / 24);
	date.Year = (int)(30 * alpha + beta - 30);
	
	return date;
}


/*
 * MJD: Modified Julian Day (MJD = 0.0 corresponds to 1858 November 17 at 0h UT.
 * NOTE: This method valid for positive and negative years, but not for negative Julian Days.
 */
double ModifiedJulianDayFromGregorian(int year, int month, float day)
{
	return (JulianDayFromGregorian(year, month, day) - 2400000.5);
}


/*
 * JD0: the Julian Day JD0 responding to January 0.0 of the given year.
 * NOTE: This method valid for positive and negative years, but not for negative Julian Days.
 */
double JulianDay0FromGregorian(int year)
{
	if (year >= 1901 && year <= 2099) /* For years between 1901 and 2099 we use this formula. */
		return (1721409.5 + (int)(365.25 * (year - 1)));
	else { /* For other years we use this one. */
		year--;
		int a = (int)(year / 100);
		return ((int)(365.25 * year) - a + (int)(a / 4) + 1721424.5);
	}
}


/* Returns the day of week */
WeekDay DayOfTheWeekFromGregorioan(int year, int month, float day)
{
	double jul = JulianDayFromGregorian(year, month, (int)day) + 1.5;
	int week_day = (int)jul % 7;
	return week_day;
}


/* Equation of Time */
double EquationOfTime(int year, int month, float day)
{
	double T, L0, M, y, epsilon, epsilon0, e;
	
	T = (JulianDayFromGregorian(year, month, day) - 2451545.0) / 36525;
	
	L0 = 280.46645 + 36000.76983 * T + 0.0003032 * T * T;
	
	// M: Mean anomaly of the sun.
	M = 357.52772 + 35999.05034 * T - 0.0001603 * T * T - (T * T * T) / 300000;
	
	// epsilon: The obliquity of the ecliptic.
	const double CTE1 = 23 + 26.0 / 60 + 21.448 / 3600;	// 23°26'21".448
	const double CTE2 = 4680.93 / 3600;					// 4680".93
	
	double U = T / 100;

	epsilon0 = CTE1 - CTE2 * U
					- 1.55 * pow(U, 2)
					+ 1999.25 * pow(U, 3)
					- 51.38 * pow(U, 4)
					- 249.67 * pow(U, 5)
					- 39.05 * pow(U, 6)
					+ 7.12 * pow(U, 7)
					+ 27.87 * pow(U, 8)
					+ 5.79 * pow(U, 9)
					+ 2.45 * pow(U, 10);
	
	// Let's the L0 and M angles between 0 and 360 degree.
	while (L0 > 0)	L0 -= 360;
	while (L0 < 0)	L0 += 360;
	while (M > 0)	M -= 360;
	while (M < 0)	M += 360;
	
	y = tan(epsilon / 2) * tan(epsilon / 2);
	
	printf("JD\t= %.2f\nT\t= %.15f\nL0\t= %.15f\nM\t= %.15f\neps0\t= %.15f\n", JulianDayFromGregorian(year, month, day), T, L0, M, epsilon0);
	
	// Return's the equation of time 
	return y * sin(2 * L0) - 2 * e * sin(M) + 4 * e * y * sin(M) * cos(2 * L0) -(1 / 2) * y * y * sin(4 * L0) - (5 / 4) * e * e * sin(2 * M);
}



int main() /* Testing */
{
	double jd = JulianDayFromGregorian(2011, 7, 16.5);
	Date gdate = GregorianDate(jd);
	Date hdate = HijriDate(jd, 0);
	float et=EquationOfTime(1987, 4, 10.0);
	printf("Equation of time = %f\n", et);
	printf("2000/1/1.5 ==> JD = %.2f\n", JulianDayFromGregorian(2000, 1, 1.5));
	printf("%.2f/%d/%d gregorian = %.2f/%d/%d hijri\n", gdate.Day, gdate.Month, gdate.Year, hdate.Day, hdate.Month, hdate.Year);
	
	printf("The sine of 152.22 is %f\n", sin(152.22));
	getch();
	return 0;

}
