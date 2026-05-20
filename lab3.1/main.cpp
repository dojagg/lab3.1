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

		amount = input_num();                                                   

		ptr = create_ptr(amount);                                                

		border_price = input_price();                                           
		choose_input(amount, ptr);

		output_all(ptr, amount);

		highest_price = high_price(amount, ptr);                                  
		names_ind = create_names_ind(amount);                                      
		names_ind = under_price(amount, ptr, border_price, &end_size, names_ind);  

		sort = choose_sort(border_price);
		out = choose_output();
		output(names_ind, highest_price, end_size, sort, out);
		free_mem(names_ind, ptr, amount);                                          
		printf("\n\nХотите продолжить? (y/Y): ");                                  
                                         	
		scanf_s(" %c", &choice, 1);

	} while (choice == 'y' || choice == 'Y');                                  

	return 0;
}