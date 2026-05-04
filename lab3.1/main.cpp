#include <stdio.h>                                           
#include <stdlib.h>
#define MAX 15                                                   // максимальное кол-во символов в названии
#include "header.h"
#include <locale.h>


int main()
{

	int amount, end_size, sort, out;
	float border_price;
	goods* ptr;
	goods* names_ind;
	goods* highest_price; // массив структур
	char choice;


	setlocale(LC_ALL, "Russian");

	do
	 {

		amount = input_num();                                                      // ввод количества товаров

		ptr = create_ptr(amount);                                                  // создание массива структур

		border_price = input_price();                                              // ввод ориентировочной цены
		choose_input(amount, ptr);
		highest_price = high_price(amount, ptr);                                   // поиск максимальной цены

		names_ind = create_names_ind(amount);                                      // выделение памяти для массива товаров
		
		names_ind = under_price(amount, ptr, border_price, &end_size, names_ind);  // поиск товаров дешевле ориентировочной цены

		sort = choose_sort();
		out = choose_output();
		output(names_ind, highest_price, end_size, sort, out);
		free_mem(names_ind, ptr, amount);                                          // освобожление выделенной памяти
		printf("\n\nХотите продолжить? (y/Y): ");                                  // вывод предложения о продолжении программы

		while (getchar() != '\n');
		scanf_s("%c", &choice, 1);                                                 // сканирование выбора пользователя

	} while (choice == 'y' || choice == 'Y');                                  

	return 0;
}