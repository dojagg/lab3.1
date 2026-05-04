#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include <string.h>
#define MAX 15
#define AGE 5
#define NAME "constructor"
#define FILE_NAME "text.txt"

void input(int amount, goods* s)                                                             // функция для заполнения полей структуры
{
	printf("Введите сведения о %d товарах: \n", amount);
	for (int i = 0; i < amount; i++)
	{
		printf("\nВведите сведения о %d товаре: \n", i + 1);
		printf("Введите название товара - ");
		s[i].name = (char*)calloc(MAX + 1, sizeof(char));                                     // выделение памяти для поля с название
		scanf_s("%s", s[i].name, MAX + 1);

		printf("Введите цену товара - ");                                                     // заполнение поля с ценой
		check_float(&s[i].price);

		printf("Введите возраст от - ");													   // заполнение поля с минимальным возрастом
		check_num(&s[i].min);

		printf("Введите возраст до - ");                                                        // заполнение поля с максимальным возрастом
		while (scanf_s("%d", &s[i].max) != 1 || s[i].max <= s[i].min)
		{
			printf("Ошибка - возраст 'до' должен быть больше возраста 'от'.\nВведите возраст до - ");
			clear_buffer();
		}

	}
}
void input_from_file(int num, goods* ptr)                                                // ввод из файла
{
	FILE* fl;
    fopen_s(&fl, "input.txt", "r");                                                      // открытие файла для чтения
	if (fl == NULL)
	{
		printf("нет файла");
		exit(1);
	}
	for (int i = 0; i < num; i++)                                                        // считывание информации о товарах
	{
		ptr[i].name = (char*)calloc(MAX + 1, sizeof(char));
		fscanf_s(fl, "%s", ptr[i].name, MAX+1);
		fscanf_s(fl, "%f", &ptr[i].price);
		fscanf_s(fl, "%d", &ptr[i].min);
		fscanf_s(fl, "%d", &ptr[i].max);
	}
	fclose(fl);                                                                           // закрытие файла
}		

goods* high_price(int amount, goods* ptr)                                                   // функция для нахождения конструктора с максимальной ценой
{
	float highest = 0;                                                                     // начальное значение для максимальной цены
	goods* highest_ptr = NULL;
	for (int i = 0; i < amount; i++)
	{
		if (strcmp(ptr[i].name, NAME) == 0)                                                // является ли товар конструктором 
		{
			if (highest == 0 || ptr[i].price > highest)
			{
				highest_ptr = &ptr[i];
				highest = ptr[i].price;

			}
		}

	}
	return highest_ptr;                                                                              // вернуть значение максимальной цены
}

int input_num()                                                                                  // функция для ввода количества товаров
{
	int num;
	printf("Введите количество товаров - ");
	check_num(&num);                                                                             // вернуть введенное число
	return num;
}

float input_price()                                                                             // функция для ввода ориетировочной цены 
{
	float price;
	printf("Введите цену для ориентира - ");
	check_float(&price);
	return price;                                                                                 // вернуть введенное число
}


goods* under_price(int amount, goods* ptr, float border_price, int* end_size, goods* names_ind)   // функция для поиска товаров, которые дешевле ориетировочной цены
{
	*end_size = 0;                                                                               // начальное значение количества подходящих товаров

	for (int i = 0; i < amount; i++)
	{
		if (ptr[i].price <= border_price && ptr[i].min <= AGE && AGE <= ptr[i].max)                // условие для отбора товаров
		{
			names_ind[*end_size] = ptr[i];
			(*end_size)++;

		}
	}
	if (*end_size > 0)                                                                              // если есть такие товары 
	{
		goods* temp = (goods*)realloc(names_ind, *end_size * sizeof(goods));
		if (temp != NULL)
		{
			names_ind = temp;
		}
	}
	else
	{
		free(names_ind);                                                                           // очистка выделенной памяти
		names_ind = NULL;
	}

	return names_ind;
}

void output(goods* names_ind, goods* ptr, int end_size, int choice, int choice1)    // вывод информации о товарах
{
	FILE* fl = NULL;
	if (choice1 == 1)                                                                                    // вывод в консоль 
	{
		fl = stdout;                                                                                     
	}
	else if (choice1 == 2)                                                                               // вывод в файл
	{
		fopen_s(&fl, FILE_NAME, "w");
		if (fl == NULL)
		{
			exit(1);
		}
	}
	if (fl == NULL)
	{
		exit(1);
	}
	if (choice == 1)                                                                                          // вывод информации о товарах
	{
		if (end_size == 0)
		{
			fprintf(fl, "\nNo products matching the price and age criteria!\n");                               // если нет подходящих товаров
		}
		else
		{
			fprintf(fl, "       Products for children %d years old with price below specified        \n", AGE); // вывод информации о товарах
			fprintf(fl, "\n  N | Name              Price        Age (from - to)      \n");

			for (int i = 0; i < end_size; i++)
			{
				fprintf(fl, " %2d | %-15s   %3.2f                 %d - %d     \n",
					i + 1,
					names_ind[i].name,
					names_ind[i].price,
					names_ind[i].min,
					names_ind[i].max);
			}
		}
	}
	else if (choice == 2)                                                                                 // вывод информации о конструкторе
	{
		if (ptr == NULL)
		{
			fprintf(fl, "\nNo constructor in the list");
		}
		else
		{
			fprintf(fl, "\nThe most expensive constructor\n");
			fprintf(fl, "       Products for children %d years old with price below specified        \n", AGE); // вывод информации о товарах
			fprintf(fl, "\n  N | Name              Price        Age (from - to)      \n");
			fprintf(fl, "      | %-15s   %3.2f                 %d - %d     \n",
				
			    (*ptr).name,
				(*ptr).price,
				(*ptr).min,
				(*ptr).max);
		}
	}
	if (choice1 == 2 && fl != NULL) 
	{
		fclose(fl);                                                                                     // закрытие файла
	}
}

int choose_sort()                                                                                      // выбор фильтра
{
	int choice = 0;
	printf("\nКакие сведения хотите получить?");
	printf("\n1. Названия товаров, у которых цена не превышает заданную, и подходят детям 5 лет.");
	printf("\n2. Цену самого дорогого конструктора.");
	printf("\nВведите номер пункта - ");
	check_num(&choice);
	return choice;
}
int choose_output()                                                                                   // выбор источника вывода
{
	int choice;
	printf("\nКуда вывести результат - ");
	printf("\n1.Консоль");
	printf("\n2.Файл");
	printf("\nВведите номер пункта - ");
	check_num(&choice);
	return choice;
}

goods* create_ptr(int amount)                                                                     // функция создания массива структур
{
	goods* ptr = (goods*)calloc(amount , sizeof(goods));                                          // выделение памяти под массив структур
	if (ptr == NULL)                                                                              // проверка выделения памяти
	{
		exit(1);
	}
	return ptr;
}

void free_mem(goods* names_ind, goods* ptr, int amount)                                            // функция для очистки памяти
{
	if (ptr != NULL) 
	{
		free(ptr); 
	}
	if (names_ind != NULL) 
	{
		free(names_ind);
	}
}

goods* create_names_ind(int amount)                                                         // функция для создания массива 
{
	goods* names_ind = (goods*)calloc(amount , sizeof(goods));                              // выделение памяти для массива индексов названий
	if (names_ind == NULL)
	{
		exit(1);
	}
	return names_ind;
}

void choose_input(int amount, goods* ptr)                                                  // выбор источника ввода
{
	int temp;
	printf("Выберите источника ввода: ");
	printf("\n1.Консоль");
	printf("\n2.Файл");
	printf("\nВведите номер пункта - ");
	scanf_s("%d", &temp);
	if (temp == 1)
	{
		input(amount, ptr);                                                                 // ввод данных о товарах из консоли
	}
	else if (temp == 2)
	{
		input_from_file(amount, ptr);                                                       // ввод из файла
	}
}
void check_num(int* num)                                                                    // проверка на положительное число
{
	while (scanf_s("%d", num) != 1 || *num <= 0)                                            // пока ввод некорректен
	{
		printf("Ошибка - введите целое, положительное целое число: ");
		clear_buffer();                                                                     // очистка буфера ввода
	}
}

void clear_buffer()                                                                         // очистка буфера ввода
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);                                            // читаем и отбрасываем символы до конца строки
}
void check_float(float* val) 
{
	while (scanf_s("%f", val) != 1 || *val <= 0) {
		printf("Ошибка - введите положительное число: ");
		clear_buffer();
	}
}