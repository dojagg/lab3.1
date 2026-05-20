#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include <string.h>
#define MAX 15
#define AGE 5
#define NAME "constructor"
#define ALL_OUTPUT "all.txt"
#define INPUT "input.txt"
#define OUTPUT "output.txt"
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
	char filename[30] = INPUT;
	while (fopen_s(&fl, filename, "r") != 0 || fl == NULL)
	{
		printf("\nФайл не найден");
		printf("\nВведите имя файла для открытия - ");
		scanf_s("%s", filename, 30);
		clear_buffer();
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

goods* high_price(int amount, goods* ptr, int* exp_size)                                                   // функция для нахождения конструктора с максимальной ценой
{                                                                   
	goods* highest_ptr = create_ptr(amount);
	*exp_size = 0;
	int count = 0;
	float highest = 0;
	for (int i = 0; i < amount; i++)
	{
		if (strcmp(ptr[i].name, NAME) == 0)                                                // является ли товар конструктором 
		{
			if (highest == 0 || ptr[i].price > highest)
			{
				highest = ptr[i].price;
				count = 0;
				highest_ptr[count] = ptr[i];
				count++;

			}
			else if (ptr[i].price == highest)
			{
				highest_ptr[count] = ptr[i];
				count++;
			}
		}

	}
	if (count > 0)
	{
		
		goods* temp = (goods*)realloc(highest_ptr, count * sizeof(goods));
		if (temp != NULL)
		{
			highest_ptr = temp;
		}
		*exp_size = count;
	}
	else
	{
	
		free(highest_ptr);
		highest_ptr = NULL;
	}

	return highest_ptr;
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
			names_ind = temp;
		
	}
	else
	{
		free(names_ind);                                                                           // очистка выделенной памяти
		names_ind = NULL;
	}

	return names_ind;
}

void output(goods* items, int size, const char* header, int output_variant, const char* filename)
{
	FILE* fl = NULL;
	if (output_variant == 1) 
		fl = stdout;
	
	else if (output_variant == 2) 
	{
		fopen_s(&fl, filename, "w");
		if (fl == NULL)
			exit(1);
	}

	if (fl == NULL)
		exit(1);

	if (size == 0 || items == NULL)
		fprintf(fl, "\nNo products matching the criteria were found!\n");
	
	else
	{
		if (header != NULL)
			fprintf(fl, "%s\n", header);

		fprintf(fl, " %2s | %-15s   %-10s   %s\n",
			"N", "Name", "Price", "Age (from - to)");

		for (int i = 0; i < size; i++)
		{
			fprintf(fl, " %2d | %-15s   %-10.2f   %d - %d\n",
				i + 1,
				items[i].name,
				items[i].price,
				items[i].min,
				items[i].max);
		}

		if (output_variant == 2)
			printf("\nWritten successfully to %s!", filename);
		
	}

	if (output_variant == 2 && fl != NULL)
		fclose(fl); 
	
}


void choose_sort(float border_price, goods* names_ind, int names_size, goods* expensive, int exp_size, int out)                                                                      // выбор фильтра
{
	int choice;
	printf("\nВарианты вывода:");
	printf("\n1.Товары для детей %d лет с ценой < %.2f", AGE, border_price);
	printf("\n2.Самый дорогой конструктор");
	printf("\nВведите 1/2 - ");
	while (scanf_s("%d", &choice) != 1 || (choice != 1 && choice != 2))
	{
		printf("Ошибка! Введите 1 или 2: ");
		clear_buffer();

	}
	clear_buffer();
	if (choice == 1)
		output(names_ind, names_size, "Items for children", out, OUTPUT);
	else if(choice ==2)
		output(expensive, exp_size, "The most expensive", out, OUTPUT);
}
int choose_output()                                                                                     // выбор источника вывода
   {	
	int choice;
	printf("\nКуда вывести результат - ");
	printf("\n1.Консоль");
	printf("\n2.Файл");
	printf("\nВведите 1/2 - ");
	while (scanf_s("%d", &choice) != 1 || (choice != 1 && choice != 2)) 
	{
		printf("Ошибка! Введите 1 или 2: ");
		clear_buffer();
		
	}
	clear_buffer();
	return choice;
}

goods* create_ptr(int amount)                                                                     // функция создания массива структур
{
	goods* ptr = (goods*)calloc(amount , sizeof(goods));                                          // выделение памяти под массив структур
	if (ptr == NULL)                                                                              // проверка выделения памяти
		exit(1);
	
	return ptr;
}

void free_mem(goods* names_ind, goods* expensive, goods* ptr, int amount)
{
	if (ptr != NULL)
	{
		for (int i = 0; i < amount; i++)
		{
			if (ptr[i].name != NULL)
			{
				free(ptr[i].name); 
				ptr[i].name = NULL;
			}
		}
		free(ptr);
	}
	if (names_ind != NULL)
		free(names_ind);
	if (expensive != NULL)
		free(expensive);
	
}

goods* create_names_ind(int amount)                                                         // функция для создания массива 
{
	goods* names_ind = (goods*)calloc(amount , sizeof(goods));                              // выделение памяти для массива индексов названий
	if (names_ind == NULL)
		exit(1);

	return names_ind;
}

void choose_input(int amount, goods* ptr)                                                  // выбор источника ввода
{
	int choice;
	printf("\nВарианты ввода:");
	printf("\n1.Консоль");
	printf("\n2.Файл");
	printf("\nВведите 1/2 - ");
	while (scanf_s("%d", &choice) != 1 || (choice != 1 && choice != 2)) 
	{
		printf("Ошибка! Введите 1 или 2: ");
		clear_buffer();
		
	}
	clear_buffer();
	if (choice == 1)
		input(amount, ptr);                                                                 // ввод данных о товарах из консоли

	else if (choice == 2)
		input_from_file(amount, ptr);                                                       // ввод из файла
	
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