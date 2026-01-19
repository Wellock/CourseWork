#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define SIZE 30


typedef struct {
	double start;
	double finale;
	double step;
	int countstep;
	double error;
} info;

double function(double x);
int build_tabl(info z, char* name, char mode);
char* monotony(info z, char* name);
double X_on_Y(double znachY, info z);
double derivative(double x, double error);
int write_doc(char* name, double numb, int chois);
int read_and_build_table(char* name);

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

		puts("\nЧто вы хотите?\n1. Значение функции в точке\n2. Таблица значений\n3. Анализ монотонности\n4. Поиск Х по Y\n5. Производная в точке\n6. Установить точность вычислений\n7. Прочитать файл и составить таблицу\n0. Выход\n");
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
			info task2;
			//float start;
			scanf("%lf", &task2.start);
			puts("Введие шаг");
			//float step;
			scanf("%lf", &task2.step);
			puts("Введие количиство отсчетов");
			//int qua;
			scanf("%i", &task2.countstep);
			build_tabl(task2, resultat, 'c');
			if (build_tabl(task2, resultat, 'f') == 0)
			{
				fprintf(stderr, "Ошибка открытия файла\n");
			}
			break;
		case 3:
			puts("Введите начало интервала");
			info task3;
			//double st_extreme;
			scanf("%lf", &task3.start);
			puts("Введите конец интервала");
			//double fi_extreme;
			scanf("%lf", &task3.finale);
			puts("Введите количество проверочных точек");
			//int quantity;
			scanf("%i", &task3.countstep);
			char mono = monotony(task3, resultat);
			switch (mono) 
			{
			case '+':
				printf("На промежутке [%.3lf, %.3lf] функция возрастает\n", task3.start, task3.finale);
				break;
			case '-':
				printf("На промежутке [%.3lf, %.3lf] функция убывает\n", task3.start, task3.finale);
				break;
			case '=':
				printf("На промежутке [%.3lf, %.3lf] функция не является монотонной\n", task3.start, task3.finale);
				break;
			case 'с':
				printf("На промежутке [%.3lf, %.3lf] функция постоянна и не является монотонной\n", task3.start, task3.finale);
				break;
			case '?':
				printf("На промежутке [%.3lf, %.3lf] не удалось определить характер функции\n", task3.start, task3.finale);
				break;
			}

			break;
		case 4:

			puts("Введите значение функции");
			info task4;
			double y;
			scanf("%lf", &y);
			puts("Введите начало интервала");
			//double start_p4;
			//double finale_p4;
			scanf("%lf", &task4.start);
			puts("Введите конец интервала");
			scanf("%lf", &task4.finale);

			task4.error = error_rate;
			if (isnan(X_on_Y(y, task4)))
			{
				printf("На данном промежутке таких значений нет\n");
			}
			else 
			{
				printf("X = %lf\n", X_on_Y(y, task4));
			}
			write_doc(resultat, X_on_Y(y, task4), 4);
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
			int res = read_and_build_table(tabl);
			switch (res)
			{
			case -1:
				fprintf(stderr, "Ошибка: не удалось открыть файл\n");
				break;
			case -2:
				printf(stderr, "Ошибка: в файле нет данных\n");
				break;
			case 0:
				printf("Таблица успешно записана\n");
				break;
			}
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

int build_tabl(info z, char* name, char mode)
{
	double x = z.start;
	FILE* out = NULL;
	if (mode == 'f')
	{
		if ((out = fopen(name, "at")) == NULL)
		{
			return 0;
		}
	}
	if (mode == 'c')
	{
		out = stdout;
	}
	fprintf(out, "| Значение х | Значение f(x) |\n");
	for (int i = 0; i < z.countstep; i++)
	{
		fprintf(out, "| %10.4g | %13.4g |\n", x, function(x));
		x = x + z.step;
	}
	if (mode == 'f')
	{
		fclose(out);
	}
	return 1;
}



char* monotony(info z, char* name)
{
	double g = (z.finale - z.start) / (z.countstep - 1);
	double first_y = function(z.start);
	_Bool up = 0;
	_Bool down = 0;
	_Bool constant = 1;
	FILE* out;
	if ((out = fopen(name, "at")) == NULL)
	{
		printf("Ошибка записи данных");
		return 0;
	}
	for (int i = 1; i < z.countstep; i++)
	{
		double x = z.start + i * g;
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
	if (constant)
	{
		fprintf(out, "На промежутке [%.3lf, %.3lf]\n Функция постоянна\n", z.start, z.finale);
		fclose(out);
		return 'c';
	}
	else if (up && !down)
	{
		fprintf(out, "На промежутке [%.3lf, %.3lf]\n Функция возрастает\n", z.start, z.finale);
		fclose(out);
		return '+' ;
	}
	else if (!up && down)
	{
		fprintf(out, "На промежутке [%.3lf, %.3lf]\n Функция убывает\n", z.start, z.finale);
		fclose(out);
		return '-';
	}
	else if (up && down)
	{
		fprintf(out, "На промежутке [%.3lf, %.3lf]\n Функция не монотонна\n", z.start, z.finale);
		fclose(out);
		return '=';
	}
	else
	{
		fprintf(out, "На промежутке [%.3lf, %.3lf]\n не удалось определить характер функции\n", z.start, z.finale);
		fclose(out);
		return '?';
	}
}

double X_on_Y( double znachY, info z)
{

	/*double f_left = function(z.start);
	double f_right = function(z.finale);
	double dist_left = f_left - znachY;
	double dist_right = f_right - znachY;
	double mid;
	double f_mid;
	for (int i = 0; i < 100000; i++)
	{
		mid = (z.start + z.finale) / 2.0;
		f_mid = function(mid);
		double dist_mid = f_mid - znachY;
		if (fabs(f_mid - znachY) < z.error)
		{
			return mid;
		}
		if (dist_left * dist_mid <= 0) 
		{

			z.finale = mid;
			dist_right = dist_mid;
		}
		else 
		{
			z.start = mid;
			dist_left = dist_mid;
		}
	}
	return NAN;*/

	double step = 0.0001;
	double x = z.start;
	_Bool pr = 0;
	while (x <= z.finale)
	{
		double fx = function(x);
		double diff = fabs(fx - znachY);
		if (diff < z.error && pr == 0)
		{

			return x;
			pr = 1;

		}
		x += step;
	}
	if (pr == 0)
	{
		return NAN;
	}
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
		if (numb == NAN)
		{
			fprintf(out, "На этом интервале таких значений нет\n");
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


int read_and_build_table(char* name)
{
	FILE* file;
	double x[SIZE];
	int count = 0;

	if ((file = fopen(name, "r")) == NULL)
	{
		return -1;
	}

	while (count < SIZE && fscanf(file, "%lf", &x[count]) == 1)
	{
		count++;
	}

	fclose(file);

	if (count == 0)
	{
		return -2;
	}

	if ((file = fopen(name, "a")) == NULL)
	{
		return -1;
	}

	fprintf(file, "\n| Значение x | Значение f(x) |\n");
	//printf("\n| Значение x | Значение f(x) |\n");
	int k = 0;

	for (int i = 0; i < count; i++)
	{
		double y = function(x[i]);
		fprintf(file, "| %10.4g | %13.4g |\n", x[i], y);
		//printf("| %10.4g | %13.4g |\n", x[i], y);
		k += 1;
	}
	if (k == count) return 0;

	fclose(file);
}
