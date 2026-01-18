#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define SIZE 30

double function(double x);
void build_tabl(float st, float step, int qua, char* name);
char* monotony(double start, double finale, int step, char* name);
double X_on_Y(double znachY, double error, double start, double fin);
double derivative(double x, double error);
int write_doc(char* name, double numb, int chois);
void read_and_build_table(char* name);

void main()

{

	system("chcp 1251");

	char resultat[20];
	puts("Введите еазвание файла для сохранения");
	scanf("%49s", resultat);


	double error_rate;
	puts("Введите точность вычислений");
	scanf("%lf", &error_rate);

	int rep = 1;
	do
	{
		int choice;

		puts("\nЧто вы хотите?\n1. Значение функции в точке\n2. Таблица значений\n3. Анализ монотонности\n4. Поиск Х по Y\n5. Производная в точке\n6. Установить точность вычислений\n7. Почитать файл и составить таблицу\n0. Выход\n");
		scanf("%i", &choice);
		switch (choice)
		{
		case 1:

			puts("Введите значение х");
			double x;
			scanf("%lf", &x);
			printf("В данной точке значение функции равно >>> %lf\n", function(x));
			write_doc(resultat, function(x), 1);
			break;
		case 2:
			puts("Введие начало отсчета");
			float start;
			scanf("%f", &start);
			puts("Введие шаг");
			float step;
			scanf("%f", &step);
			puts("Введие количиство отсчетов");
			int qua;
			scanf("%i", &qua);
			build_tabl(start, step, qua, resultat);
			break;
		case 3:
			puts("Введите начало интервала");
			double st_extreme;
			scanf("%lf", &st_extreme);
			puts("Введите конец интервала");
			double fi_extreme;
			scanf("%lf", &fi_extreme);
			puts("Введите количество проверочных точек");
			int quantity;
			scanf("%i", &quantity);
			puts(monotony(st_extreme, fi_extreme, quantity, resultat));
			break;
		case 4:

			puts("Введите значение функции");
			double y;
			scanf("%lf", &y);
			puts("Введите начало интервала");
			double start_p4;
			double finale_p4;
			scanf("%lf", &start_p4);
			puts("Введите конец интервала");
			scanf("%lf", &finale_p4);

			printf("X = %lf\n", X_on_Y(y, error_rate, start_p4, finale_p4));
			write_doc(resultat, X_on_Y(y, error_rate, start_p4, finale_p4), 4);
			break;


		case 5:
			puts("Введите значение х");
			double p;
			scanf("%lf", &p);
			printf("Значение производной равно %lf\n", derivative(p, error_rate));
			write_doc(resultat, derivative(p, error_rate), 5);
			break;

		case 6:
			puts("Введите новую точность вычислений\n");
			scanf("%lf", &error_rate);
			break;
		case 7:
			puts("Введите название файла\n");
			char tabl[21];
			scanf("%99s", tabl);
			read_and_build_table(tabl);
			break;
		case 0:
			rep = 0;
			break;
		}


	} while (rep == 1);


	

}



double function(double x)
{
	double y;
	if (x < 2) y = (pow(tan(x), -1) - sin(x)) / pow(x, 3);
	if (x >= 2 && x < 9) y = (exp(cos(x)) * sqrt(pow(x, 2) + 1)) / log(3 + x);
	if (x >= 9) y = sin(2 * x - 5) / (1 + pow(x, 6));
	return y;
}

void build_tabl(float st, float step, int qua, char* name)
{
	FILE* out;
	if ((out = fopen(name, "at")) == NULL)
	{
		printf("Ошибка записи данных");
		return 0;
	}
	puts("| Значение х | Значение f(x) |");
	for (int i = 0; i < qua; i++, st = st + step)
	{
		printf("| %10.4g | %13.4g |\n", st, function(st));
		fprintf(out, "| %10.4g | %13.4g |\n", st, function(st));
	}
	fclose(out);
}



char* monotony(double start, double finale, int step, char* name)
{
	double g = (finale - start) / (step - 1);
	double first_y = function(start);
	_Bool up = 0;
	_Bool down = 0;
	_Bool constant = 1;
	FILE* out;
	if ((out = fopen(name, "at")) == NULL)
	{
		printf("Ошибка записи данных");
		return 0;
	}
	for (int i = 1; i < step; i++)
	{
		double x = start + i * g;
		double second_y = function(x);
		if (second_y > first_y)
		{
			up = 1;
			constant = 0;
		}
		if (second_y < first_y)
		{
			down = 1;
			constant = 0;
		}
		first_y = second_y;
	}
	printf("Анализ монотонности на [%lf, %lf]: \n", start, finale);
	if (constant)
	{
		fprintf(out, "На данном промежутке\n");
		fprintf(out, "Функция постоянно на данном отрезке\n");
		fclose(out);
		return "Функция постоянно на данном отрезке";
	}
	else if (up && !down)
	{
		fprintf(out, "На данном промежутке\n");
		fprintf(out, "Функция монотонно возрастает\n");
		fclose(out);
		return "Функция монотонно возрастает";
	}
	else if (!up && down)
	{
		fprintf(out, "На данном промежутке\n");
		fprintf(out, "Функция монотонно убывает\n");
		fclose(out);
		return "Функция монотонно убывает";
	}
	else if (up && down)
	{
		fprintf(out, "На данном промежутке\n");
		fprintf(out, "Функция не является монотонной\n");
		fclose(out);
		return "Функция не является монотонной";
	}
	else
	{
		fprintf(out, "На данном промежутке\n");
		fprintf(out, "Не удалось определить характер функции\n");
		fclose(out);
		return "Не удалось определить характер функции";
	}
}

double X_on_Y( double znachY, double error, double start, double fin)
{
	double step = 0.00001;
	double x = start;
	_Bool pr = 0;
	while (x <= fin)
	{
		double fx = function(x);
		double diff = fabs(fx - znachY);
		if (diff < error && pr == 0)
		{

			return x;
			pr = 1;

		}
		x += step;
	}
	if (pr == 0)
	{
		printf("На данном промежутке таких значений нет\n");
	}
	return 0;
}




double derivative(double x, double error)
{
	double h = error;
	double y = function(x+h)-function(x);
	return y;
}


int write_doc(char* name, double numb, int chois)
{
	FILE* out;
	if ((out = fopen(name, "at")) == NULL)
	{
		printf("Ошибка записи данных");
		return 0;
	}
	if (chois == 1)
	{
		printf("Производится запись значения в точке\n");
		fprintf(out, "Запись значения в точке\n");
		fprintf(out, "%lf\n", numb);
		fclose(out);
		return 1;
	}
	if (chois == 4)
	{
		printf("Производится запись найденного X по Y\n");
		fprintf(out, "Запись значения X по Y\n");
		if (numb == 0)
		{
			fprintf(out, "На этом интервале таких значений нет");
			fclose(out);
		}
		else
		{
			fprintf(out, "%lf\n", numb);
			fclose(out);
		}
		return 1;
	}
	if (chois == 5)
	{
		printf("Производится запись производной в точке\n");
		fprintf(out, "Запись производной\n");
		fprintf(out, "%lf\n", numb);
		fclose(out);
		return 1;
	}
	else
	{
		printf("Запись не удалась");
		fclose(out);
		return 0;
	}
}


void read_and_build_table(char* name)
{
	FILE* file;
	double x[SIZE];
	int count = 0;

	if ((file = fopen(name, "r")) == NULL)
	{
		printf("Ошибка открытия файла для чтения\n");
		return;
	}

	while (count < SIZE && fscanf(file, "%lf", &x[count]) == 1)
	{
		count++;
	}

	fclose(file);

	if (count == 0)
	{
		printf("В файле нет числовых значений\n");
		return;
	}

	if ((file = fopen(name, "a")) == NULL)
	{
		printf("Ошибка открытия файла для записи\n");
		return;
	}

	fprintf(file, "\n| Значение x | Значение f(x) |\n");
	printf("\n| Значение x | Значение f(x) |\n");

	for (int i = 0; i < count; i++)
	{
		double y = function(x[i]);
		fprintf(file, "| %10.4g | %13.4g |\n", x[i], y);
		printf("| %10.4g | %13.4g |\n", x[i], y);
	}

	fclose(file);
}
