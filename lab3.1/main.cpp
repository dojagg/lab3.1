#include <stdio.h>                                           
#include <stdlib.h>
#define MAX 15                                                   // максимальное кол-во символов в названии
#include "header.h"
#include <locale.h>
#define ALL_OUTPUT "all.txt"
#define INPUT "input.txt"
#define OUTPUT "output.txt"
int main()
{

	int amount, names_size, out, exp_size;
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

		out = 1;
		output(ptr, amount, "All items", out, ALL_OUTPUT);

		highest_price = high_price(amount, ptr, &exp_size);                                  
		names_ind = create_names_ind(amount);                                      
		names_ind = under_price(amount, ptr, border_price, &names_size, names_ind);  

		out = choose_output();
		choose_sort(border_price,names_ind,names_size, highest_price, exp_size, out);
		
		free_mem(names_ind, highest_price, ptr, amount);                                          
		printf("\n\nХотите продолжить? (y/Y): ");                                  
                                         	
		scanf_s(" %c", &choice, 1);

	} while (choice == 'y' || choice == 'Y');                                  

	return 0;
}