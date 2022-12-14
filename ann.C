

#include <stdio.h>
#include <fstream>
#include <iostream>


using namespace std;

char text[200], numbers[200], temp[40][15];
int q[100];

//jg jl
//Сортировка прямым способом
void sort(int length) {
	int i = length;
	__asm {
		lea edi, q; edi = указатель на массив
		mov ecx, i; ecx = количество элементов
		a0 : lea ebx, [edi + ecx * 4]; ebx = максимальный индекс в проходе + 1
		mov eax, [edi]; eax = min = величина первого элемента в проходе
		a1 : sub ebx, 4; двигаемся по проходу вверх
		cmp eax, [ebx]
		jl a2; min > array[ebx] ?
		xchg eax, [ebx]; swap(array[ebx], min)
		a2:     cmp ebx, edi
		jnz a1; проход закончился ?
		stosd; mov[edi], eax edi = +4 на первой позиции минимальный элемент
		loop a0
	}

}

//Конвертация чисел в формате строки в формат числовой(int)
void convert_char_to_int(int count) {
	int l;
	int res = 0, flag = 0, k = 0;
	char t[15], f[15];
	for (int i = 0; i < count; i++)
	{
		res = 0;
		l = 0;
		flag = 0;
		do {		//Записываем в временную переменную символы, а также считаем количество цифр в числе
			t[l] = temp[i][l];
			l++;

		} while (temp[i][l] != '\0');


		if (t[0] == '-') flag = 1; //	Если число отрицательное - возводим флаг
		if (flag == 1)
		{
			for (int i = 1; i < l; i++)
			{
				f[i - 1] = t[i];				//Записываем число в временный char массив для сдвига на 1 символ вправо (чтобы убрать минус)
			}
			for (int i = 0; i < l; i++)
			{
				t[i] = f[i];	//Записываем число уже без минуса
			}
			l--; //Так как минус за число не считается, уменьшаем общее число элементов
		}

		_asm {
			lea esi, t
			mov ecx, l
			dec ecx
			mov eax, 1
			lp:
			xor edx, edx
				mov dl, [esi + ecx]
				push eax
				sub dl, 30h
				mul edx
				add res, eax
				pop eax
				mov edx, 10
				mul edx
				dec ecx
				cmp ecx, -1
				jnz lp
		}
		q[i] = flag == 1 ? res * -1 : res; //Записываем в массив типа int значение. Если флаг возведен, то число отрицательное, иначе- положительное
	}

}

int WRITING_FROM_A_FILE(int count) {
	int i = 0, i1;
	_asm {
		lea edi, text
		lea esi, numbers; загрузка адресов
		mov ecx, count

		mov ebx, 0
		mov edx, 0

		k:
		mov al, [edi]; берем элемент из массива text
			inc edi
			cmp al, '0'; сравниваем с 0
			jge k2; Если больше или равен, то переходим на k2, в ином случае - k5
			jmp k5

			k2 :
		cmp al, '9'; сравниваем с 9
			jle k3; Если больше или равен, то переходим на k3, в ином случае - k5
			jmp k5

			k3 : ; В данной метке происходит запись элемента в массив numbers
			push eax
			mov al, [edi - 2]
			cmp al, '-'
			jnz k4
			mov[esi + ebx], al
			inc ebx
			k4 :
		pop eax
			mov[esi + ebx], al
			inc ebx
			jmp loop1


			k5 : ; В данной метке происходит добавление пробела между двумя цифрами
			cmp[esi + ebx - 1], ' '; Если пробел уже был добавлен, то прыгаем на следующую иттерацию
			jz loop1
			cmp ebx, 0
			jz loop1
			mov[esi + ebx], ' '
			inc ebx
			inc edx
			jmp loop1

			loop1 :
		loop k

			cmp[esi + ebx - 1], ' '; проверяем последний элемент на пробел
			jnz e; если нет, то ничего не делаем, иначе заменяем элемент на '\0'
			mov al, '\0'
			mov[esi + ebx - 1], al

			e :
		mov i, edx
			mov i1, ebx; В регистре ebx хранится информация о количестве символов в массиве numbers
	}

	int k = 0, l = 0;
	//Проверка на отсутствие цифр
	if (numbers[0] == '\0') {
		cout << "В файле отсутствуют цифры" << endl;
		return 0;
	}
	cout << "Данные из файла:" << endl;

	//Вывод элементов в консоль, и размещение их в двумерный массив
	for (int i = 0; i < i1; i++)
	{
		if (numbers[i] == ' ')
		{
			cout << k + 1 << " элемент: " << temp[k] << endl;
			k++;
			l = 0;
			continue;
		}
		temp[k][l] = numbers[i];
		l++;
	}
	cout << k + 1 << " элемент: " << temp[k] << endl;

	convert_char_to_int(k + 1);

	sort(k + 1);

	ofstream file__vivod("вывод.txt");

	int tem[100];
	int c = 1;
	cout << "Отсортированные:" << endl;
	i = 0;
	for (int i = 0; i < k+1; i++)
	{
		cout << i+1 << " элемент: " << q[i] << endl;
		file__vivod << q[i] << endl;
	}
	/*do
	{
		tem[i] = q[i];
		i++;
	} while (q[i] < 0);
	int asd = i;
	for (i; i < k + 1; i++)
	{
		cout << c << " элемент: " << q[i] << endl;
		file__vivod << q[i] << endl;
		c++;
	}
	for (int l = 0; l < asd; l++)
	{
		cout << c << " элемент: " << tem[l] << endl;
		file__vivod << tem[l] << endl;
		c++;
	}*/


	printf("Отсортированные данные сохранены в файле вывод.txt\n");
	file__vivod.close();

	return i1;

}


int main()
{
	int i = 0;

	setlocale(LC_ALL, "Russian");
	fstream file("ввод.txt");

	bool isFileNotOpen = file.is_open() ? false : true;

	//Проверка на успешное открытие файла
	if (isFileNotOpen) {
		printf("Не удается найти файл ввод.txt. Возможно, его не существует\n");
		file.close();
		return 0;
	}

	char symbol;
	//Запись из текстового файла символов
	while (file) {
		file.get(symbol);
		text[i] = symbol;
		i++;
	}

	file.close();

	if (i == 1)
	{
		printf("В файле нет символов\n");
		system("pause");
		return 0;
	}

	int q = WRITING_FROM_A_FILE(i - 1);
	system("pause");
	return 0;
}
