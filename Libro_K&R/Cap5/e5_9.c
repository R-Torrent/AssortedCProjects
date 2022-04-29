/* ejercicio 5_9 */
/* 'day_of_year' y 'month_day' con apuntadores */

static char daytab[][13] = {
	{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
};

/* day_of_year: obtiene dia del ano a partir de mes y ano */
int day_of_year(int year, int month, int day)
{
	int i, leap;
		
	leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
	for(i = 1; i < month; i++)
		day += *(*(daytab + leap) + i);
	return day;
}

/* month_day: obtiene mes y dia a partir del dia del ano */
void month_day(int year, int yearday, int *pmonth, int *pday)
{
	int i, leap;

	leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
	for(i = 1; yearday > *(*(daytab + leap) + i); i++)
		yearday -= *(*(daytab + leap) + i);
	*pmonth = i;
	*pday = yearday;
}