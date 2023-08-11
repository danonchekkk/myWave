#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <array>
#include <windows.h>

HANDLE hConsole;

const int sizeDrpX = 20;
const int sizeDrpY = 20;

void printDrp(int drp[sizeDrpX][sizeDrpY]) //начальная ДРП, из файла
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//для смены цветов в консоли

	std::cout << "'" << (char)95 << "'" << " - floor \n";
	std::cout << "'" << (char)35 << "'" << " - wall\n\n";
	for (int i = 0; i < sizeDrpX; ++i)
	{
		for (int j = 0; j < sizeDrpY; ++j)
		{
			if (drp[i][j] == 0)
				std::cout << std::setw(3) << '_' << " ";
			else if (drp[i][j] == -2)
			{
				SetConsoleTextAttribute(hConsole, 8);
				std::cout << std::setw(3) << '#' << " ";
				SetConsoleTextAttribute(hConsole, 7);
			}
			else if (drp[i][j] == -3) 
			{
				SetConsoleTextAttribute(hConsole, 2);
				std::cout << std::setw(3) << 'A' << " ";
				SetConsoleTextAttribute(hConsole, 7);

			}
			else if (drp[i][j] == -4) 
			{
				SetConsoleTextAttribute(hConsole, 4);
				std::cout << std::setw(3) << 'B' << " ";
				SetConsoleTextAttribute(hConsole, 7);
			}
			else
				std::cout << std::setw(3) << drp[i][j] << " ";
		}
		std::cout << "\n";
	}
}

//перегруженная функция вывода ДРП, но уже с расставленными точками и путем
void printDrp(int drp[sizeDrpX][sizeDrpY], int aX, int aY, int bX, int bY)
{
	int waveCount = 0; //счетчик волн
	bool hasChanged = true;

	drp[aX - 1][aY - 1] = -3; // Точка А имеет значение -3
	drp[bX - 1][bY - 1] = -4; // Точка Б имеет значение -4

	if (drp[aX - 2][aY - 1] == -4 || drp[aX][aY - 1] == -4 || drp[aX - 1][aY - 2] == -4 || drp[aX - 1][aY] == -4)
		hasChanged = false;//перед началом распростанения волн фиксируем случай если точки находятся в упор друг к другу

	while (hasChanged)
	{
		hasChanged = false;
		//перебираем все элементы ДРП
		for (int i = 0; i < sizeDrpX; ++i)
		{
			for (int j = 0; j < sizeDrpY; ++j)
			{

				if (drp[i][j] == -3 && waveCount == 0)
				{ // проверяем соседей для точки А
					if (i + 1 < sizeDrpX && drp[i + 1][j] == 0 && drp[i][j] != -2)
					{
						drp[i + 1][j] = waveCount + 1;
						hasChanged = true;
					}
					if (j + 1 < sizeDrpY && drp[i][j + 1] == 0 && drp[i][j] != -2)
					{
						drp[i][j + 1] = waveCount + 1;
						hasChanged = true;
					}
					if (i - 1 > 0 && drp[i - 1][j] == 0 && drp[i][j] != -2)
					{
						drp[i - 1][j] = waveCount + 1;
						hasChanged = true;
					}
					if (j - 1 > 0 && drp[i][j - 1] == 0 && drp[i][j] != -2)
					{
						drp[i][j - 1] = waveCount + 1;
						hasChanged = true;
					}
				}

				//проверяем остальые точки
				if (drp[i][j] == waveCount && waveCount != 0)
				{
					if (i + 1 < sizeDrpX && drp[i + 1][j] == 0 && drp[i][j] != -2)
					{
						drp[i + 1][j] = waveCount + 1;
						hasChanged = true;
					}
					if (j + 1 < sizeDrpY && drp[i][j + 1] == 0 && drp[i][j] != -2)
					{
						drp[i][j + 1] = waveCount + 1;
						hasChanged = true;
					}
					if (i - 1 >= 0 && drp[i - 1][j] == 0 && drp[i][j] != -2)
					{
						drp[i - 1][j] = waveCount + 1;
						hasChanged = true;
					}
					if (j - 1 >= 0 && drp[i][j - 1] == 0 && drp[i][j] != -2)
					{
						drp[i][j - 1] = waveCount + 1;
						hasChanged = true;
					}
				}
				if (drp[bX - 1][bY - 1] == -4) //проверка на момент, когда мы уже дошли до В, и нужно остановить распространение волн
				{
					if (bX < sizeDrpX && drp[bX][bY-1] == waveCount+1 && drp[bX][bY-1] != -2)
						hasChanged = false;
					if (bY < sizeDrpY && drp[bX-1][bY] == waveCount+1 && drp[bX-1][bY] != -2)
						hasChanged = false;
					if (bX >= 0 && drp[bX - 2][bY - 1] == waveCount+1 && drp[bX - 2][bY-1] != -2)
						hasChanged = false;
					if (bY >= 0 && drp[bX - 1][bY - 2] == waveCount+1 && drp[bX - 1][bY-2] != -2)
						hasChanged = false;
				}
			}
		}

		//инкрементируем волну
		waveCount++;
	}

	//печатаем конечную ДРП
	std::cout << std::endl;
	printDrp(drp);

	//рассчитываем расстояние от А до В и выводим длину пути
	 int lenght = 1000;
	//все 4 соседа точки В образуют массив:
	std::array <int, 4> neighboursB = {drp[bX - 2][bY - 1], drp[bX - 1][bY - 2], drp[bX - 1][bY], drp[bX][bY - 1]};
	for (const auto value : neighboursB)
	{

		if (value < lenght && value < 1000 && value > 0)
			lenght = value+1;
	}

	if(drp[bX-2][bY-1] == -3 || drp[bX][bY - 1] == -3 || drp[bX - 1][bY - 2] == -3 || drp[bX - 1][bY] == -3)//проверка, вдруг А и В рядом
		std::cout << "\nPoints are close to each other(Lenght path < 1)" << std::endl;
	else if(lenght == -2 || lenght==1000)
		std::cout << "\nCouldn't find a path" << std::endl;
	else
	{
		if (lenght > waveCount)
			std::cout << "\nPath lenght is " << lenght << " cells" << std::endl;
		if (waveCount >= lenght) //чтобы не багалось
			std::cout << "\nPath lenght is " << waveCount + 1 << " cells" << std::endl;
	}
}

int coordsPoint(int pointCoord)
{
	std::cin >> pointCoord;
	while (!std::cin.good() || pointCoord < 1 || pointCoord > 20)
	{
		std::cout << "AGAIN([1;20]): ";
		std::cin.clear();
		std::cin.ignore(1024, '\n');
		std::cin >> pointCoord;
	}
	
	return pointCoord;
}

int main(int argc, char* argv[])
{				
	//инициализация самой ДРП
	int drp[sizeDrpX][sizeDrpY] = {};
	
	//вытаскиваем из файла расстановку дрп, но сперва проверка 
	std::fstream file;
	if (argc<2)
	{
		std::cout << "Not enough arguments passed.\n";
		return 0;
	}
	file.open(argv[1]);
	if (!file.is_open())
	{
		std::cout << "Invalid file.";
		std::cout<<" Try to choose another file.\n\n";
		return 0;
	}
	else
	{
		for (int i = 0; i < sizeDrpX; ++i)
		{
			for (int j = 0; j < sizeDrpY; ++j)
			{
				file >> drp[i][j];
			}
		}
	}

	file.close();

	//вывод дрп
	printDrp(drp);

	int aX = 0;
	int aY = 0;
	int bX = 0;
	int bY = 0;

	//запрашиваем у пользователя координаты точек А и В

	do //проверка, чтобы случайно не поставить точку А в стену
	{
		std::cout << "!!!Point cant be on a whall!!!\n";
		std::cout << "\nEnter the coordinates point A\nX - coordinates: ";
		aX = coordsPoint(aX);
		std::cout << "Y - coordinates: ";
		aY = coordsPoint(aY);
	} while (drp[aY-1][aX-1] == -2);

	do //проверка, чтобы у точек были различные координаты и чтобы В не поставить в стену
	{
		std::cout<<"Points must have different coordinates\n";
		std::cout << "!!!Point cant be on a whall!!!\n";
		std::cout << "\n\nEnter the coordinates point B\nX - coordinates: ";
		bX = coordsPoint(bX);
		std::cout << "Y - coordinates: ";
		bY = coordsPoint(bY);
		std::cout << std::endl;
	} while ((aX == bX && aY == bY) || (drp[bY-1][bX-1]) == -2);

	std::swap(aX, aY);//ибо они не совсем корректно отображаются
	std::swap(bX, bY);

	//вывод дрп с точками А и В
	printDrp(drp, aX, aY, bX, bY);
	
	return 0;
}
