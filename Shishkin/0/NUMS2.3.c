#include <stdio.h>
#include <string.h>
void CharToB1 (char sIN[], float nIN[], int amount) {
	int i, z;
	z = 0;
	for (i=0; i<amount; i++) {
		if (((sIN[i]-48) < 10) && ((sIN[i]-48) >= 0))  nIN[i] = sIN[i] - 46;
		if (((sIN[i]-65) < 6) && ((sIN[i]-65) >= 0))  nIN[i] = sIN[i] - 53;              //запись из аскикода
		if (((sIN[i]-97) < 6) && ((sIN[i]-97) >= 0))  nIN[i] = sIN[i] - 85;
		if (sIN[i] == 46) nIN[i] = 1;
		}
}

void fallOn2 (float nIN[], int amount) {
	int i;													//вычитает из каждого элемента массива 2
	for (i=0; i<amount; i++)	nIN[i] = nIN[i] - 2;
} 

int check (float nIN[0], int b1, int b2, int amount) {
	int i, erroramount;
	erroramount = 0;
	for (i=0; i<amount; i++) {
		if (nIN[i] == 0) erroramount++;                      //является ли число б1ричной записью и все ли коэф-ты перевелись
		if (nIN[i]>b1+1) erroramount++;
		}
	if (b1<2) erroramount++;
	if (b1>16) erroramount++;                              //лежат ли б1 и б2 в пределах от 2 до 16
	if (b2<2) erroramount++;
	if (b2>16) erroramount++;
	if (erroramount != 0) return 1;
		else return 0;
}

long long int power (int a, int n) {
	int i;
	long long int t;	
	t=a;									//возводит число а в степень п
	for (i=1; i<n; i++) {
		t = t*a;
		}
	return t;
}

int pointCheck (int pointplace, int amount, int pointcount) {
	int result;
	if (pointplace == amount-1) {
		result = -1;                      									//точка на последнем месте
		}
	if (pointplace == 0) {
		result = -1;                  										//точка на первом (нулевом) месте
		}
	if (pointplace == amount + 1) result = 1;								//нет точки
	if ((pointplace < amount - 1) && (pointplace > 0)) result = 0;			//точка не по краям
	if (pointcount > 1) result = -1;										//больше 1 точки
	return result;
}

long long int B1ToTen (float nIN[], double number1[], int b1, int amount) {
	int pointplace, i, pointcount, result;
	pointplace = amount + 1;
	pointcount = 0;
	double partnum1;
	long long int number2, partnum2;
	for	(i=0; i<amount; i++) {
		if (nIN[i] == -1) {
			pointplace = i;
			pointcount++;
			}
		}
	result = pointCheck(pointplace, amount, pointcount);
	if (result == 1) {
		number2 = nIN[amount-1];                                           
		for (i=amount-2; i>=0; i--) {
			partnum2 = nIN[i]*power(b1, amount - 1 - i);
			number2 = number2 + partnum2;
			}
		number1[0] = -2;
		}
	if (result == 0) {
		number2 = nIN[pointplace-1];
		for (i=pointplace-2; i>=0; i--) {
			partnum2 = nIN[i] * power(b1, pointplace-1-i);
			number2 = number2 + partnum2;                        					
			}
		for (i=pointplace+1; i<amount; i++) {
			partnum1 = nIN[i]/power(b1, i-pointplace);
			number1[0] = number1[0] + partnum1;
			}
		}
	if (result == -1) number2 = result;
	return number2;
}

void reverseArray (int array[], int amount) {
	int i, t;
	for (i=0; i <= amount/2; i++) {                                     //меняет местами по 2 элемента массива, начиная с краев
		t = array[i];
		array[i] = array[amount-i];
		array[amount-i] = t;
		}
}

void TenToB2 (int nOUT[], double number1[], long long int number2, int b2) {
	long long int i, left, quot, pointplace;
	i = 0;
	double number11;
	number11 = number1[0];
	while (number2 >= b2) {
		quot = number2/b2;
		left = number2 - quot*b2;
		nOUT[i] = left;                     //перевод целой части числа
		i++;
		number2 = quot;
		}
	nOUT[i] = number2;
	reverseArray(nOUT, i);
	if (number1 != -2) {
		i++;
		nOUT[i] = -1;
		pointplace = i;
		do {                                                                 //перевод дробной части числа
			i++;
			number11 = number11*b2;
			nOUT[i] = number11;
			number11 = number11 - nOUT[i];
			} while (number11 > 0);
		}
	if ((i-pointplace) > 12) nOUT[pointplace+13] = -2;
	else nOUT[i+1] = -2;
}

void B2ToChar (int nOUT[], char sOUT[], FILE *fout) {
	int i;
	i = 0;
	while (nOUT[i] != -2) {                                                         //запись в аскикод + вывод
		if (nOUT[i] < 10) sOUT[i] = nOUT[i] + 48;
		if (nOUT[i] >= 10) sOUT[i] = nOUT[i] + 55;
		if (nOUT[i] == -1) sOUT[i] = nOUT[i] + 47; 
		fprintf(fout, "%c", sOUT[i]);
		i++;
		}
}

int main () {
	FILE *fin, *fout;
	char symbolsIN[13];
	char symbolsOUT[1000];
	float numberIN[13];
	int numberOUT[1000];
	double number1[0];
	long long int number2;
	int  b1, b2, amount;
	fout = fopen("out.txt", "w");
	fin=fopen("in.txt", "r");
	if (fin == NULL) 
		fprintf(fout, "File could not be opened.");
	else {
    	fscanf(fin, "%d", &b1);
		fscanf(fin, "%d", &b2);							//считывает данные из файла			
		fscanf(fin, "%s", symbolsIN);
		amount= strlen(symbolsIN);
		CharToB1(symbolsIN, numberIN, amount);
    	if (check(numberIN, b1, b2, amount) == 1) fprintf (fout, "bad input");
    	else {
			fallOn2(numberIN, amount); 
			number2 = B1ToTen(numberIN, number1, b1, amount);
			if (number2 == -1) fprintf(fout, "bad input");
			else{
				TenToB2(numberOUT, number1, number2, b2);
				B2ToChar(numberOUT, symbolsOUT, fout);
				}
			}
		}
	fclose(fin);
	fclose(fout);
	return 0; 
}
