#pragma once


struct goods                                                                                  // стркуктура товара
{
	char *name;
	float price;
	int min;
	int max;
};

void input(int amount, goods* s);                                                             // заполнение полей структуры
goods* high_price(int amount, goods* ptr);                                                     // поиск максимальной цены
int input_num();                                                                              // ввод количества товаров
float input_price();                                                                          // ввод ориентировочной цены             
goods* under_price(int amount, goods* ptr, float border_price, int* end_size, goods* names_ind);  // поиск товаров ниже ориентировочной цены
goods* create_ptr(int amount);                                                                // создание массива указателей
void free_mem(goods* names_ind, goods* ptr, int amount);                                      // очистка памяти
goods* create_names_ind(int amount);                                                          // создание массива структур отобранных товаров 
void output(goods* names_ind, goods* ptr, int end_size, int choice, int choice1);             // вывод
int choose_sort(float border_price);                                                           // выбор фильтра
int choose_output();                                                                           // выбор источника вывода
void input_from_file(int num, goods* ptr);                                                     // ввод из файла
void choose_input(int amount, goods* ptr);                                                     // выбор источника ввода
void check_num(int* num);                                                                      // проверка на положительное число
void check_float(float* val);                                                                  // проверка дробного числа
void clear_buffer();
void output_all(goods* s, int amount);