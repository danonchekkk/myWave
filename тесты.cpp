#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <array>
#include <windows.h>

HANDLE hConsole;

int lenght;
int cnt;
int waveCount;

const int sizeDrpX = 25;
const int sizeDrpY = 25;

void printDrp(int drp[sizeDrpX][sizeDrpY])
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//для смены цветов в консоли

	std::cout << "'" << (char)46 << "'" << " - floor \n";
	std::cout << "'" << (char)37 << "'" << " - wall\n\n";
	for (int i = 0; i < sizeDrpX; ++i)
	{
		for (int j = 0; j < sizeDrpY; ++j)
		{
			switch (drp[i][j])
			{
			case 0: //пустая клетка (пол)
				std::cout << std::setw(2) << '.' << " ";
				break;

			case -2: //стена
				SetConsoleTextAttribute(hConsole, 8);
				std::cout << std::setw(2) << '%' << " ";
				SetConsoleTextAttribute(hConsole, 7);
				break;

			case -3: //точка А
				SetConsoleTextAttribute(hConsole, 2);
				std::cout << std::setw(2) << 'A' << " ";
				SetConsoleTextAttribute(hConsole, 7);
				break;

			case -4: //точка В
				SetConsoleTextAttribute(hConsole, 4);
				std::cout << std::setw(2) << 'B' << " ";
				SetConsoleTextAttribute(hConsole, 7);
				break;

			case -5://дорожка обратного пути
				SetConsoleTextAttribute(hConsole, 8);
				std::cout << std::setw(2) << '$' << " ";
				SetConsoleTextAttribute(hConsole, 7);
				break;

			default:
				std::cout << std::setw(2) << drp[i][j] << " ";
				break;
			}
		}
		std::cout << "\n";
	}
}

//перегруженная функция вывода ДРП, но уже с расставленными точками и путем
int changeDrp(int drp[sizeDrpX][sizeDrpY], int aX, int aY, int bX, int bY)
{
	waveCount = 0; //счетчик волн
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
					if (bX < sizeDrpX && drp[bX][bY - 1] == waveCount + 1 && drp[bX][bY - 1] != -2)
						hasChanged = false;
					if (bY < sizeDrpY && drp[bX - 1][bY] == waveCount + 1 && drp[bX - 1][bY] != -2)
						hasChanged = false;
					if (bX >= 0 && drp[bX - 2][bY - 1] == waveCount + 1 && drp[bX - 2][bY - 1] != -2)
						hasChanged = false;
					if (bY >= 0 && drp[bX - 1][bY - 2] == waveCount + 1 && drp[bX - 1][bY - 2] != -2)
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
	lenght = 1000;
	//все 4 соседа точки В образуют массив:
	std::array <int, 4> neighboursB = { drp[bX - 2][bY - 1], drp[bX - 1][bY - 2], drp[bX - 1][bY], drp[bX][bY - 1] };
	for (const auto value : neighboursB)
	{

		if (value < lenght && value < 1000 && value > 0)
			lenght = value + 1;
	}

	if (drp[bX - 2][bY - 1] == -3 || drp[bX][bY - 1] == -3 || drp[bX - 1][bY - 2] == -3 || drp[bX - 1][bY] == -3)//проверка, вдруг А и В рядом
	{
		std::cout << "\nPoints are close to each other(Lenght path < 1)" << std::endl;
		return 0;
	}
	else if (lenght == -2 || lenght == 1000)
	{
		std::cout << "\nCouldn't find a path" << std::endl;
		return 0;
	}
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
	while (!std::cin.good() || pointCoord < 1 || pointCoord > 25)
	{
		std::cout << "AGAIN([1;25]): ";
		std::cin.clear();
		std::cin.ignore(1024, '\n');
		std::cin >> pointCoord;
	}

	return pointCoord;
}

//функция выводы конечного пути
int printPath(int drp[sizeDrpX][sizeDrpY], int aX, int aY, int bX, int bY, std::ofstream &file)
{
	std::cout << "\nWould you like to see the completed path?\n 1-yes, 2-no\n";

	int choose = 0;
	std::cin >> choose;
	while (!std::cin.good() || (choose != 1 && choose != 2))
	{
		std::cout << "AGAIN '1' or '2' ";
		std::cin.clear();
		std::cin.ignore(1024, '\n');
		std::cin >> choose;
	}

	bool endPath = true;
	cnt = 0;
	
	switch (choose)
	{
	case 1:
		//алгоритм проставления маршрута
		while (cnt<waveCount)
		{
			cnt++;
			for (int i = 0; i < sizeDrpX; ++i)
			{
				for (int j = 0; j < sizeDrpY; ++j)
				{
					if (drp[i][j] == -4 && cnt == 1)//ищем минимального соседа вокруг точки В(если он есть) и от него будем плясать
					{
						if (i + 1 < sizeDrpX && drp[bX][bY - 1] > 0)
						{
							drp[bX][bY - 1] = -5;
						}
						else if (i - 1 >= 0 && drp[bX - 2][bY - 1] > 0)
						{
							drp[bX - 2][bY - 1] = -5;
						}
						else if (j - 1 >= 0 && drp[bX - 1][bY - 2] > 0)
						{
							drp[bX - 1][bY - 2] = -5;
						}
						else if (j + 1 < sizeDrpY  && drp[bX - 1][bY] > 0)
						{
							drp[bX - 1][bY] = -5;
						}
					}
					//а тут уже пляшем
					else if(drp[i][j] == -5 && cnt != 1)
					{	
						if (i - 1 >= 0 && drp[i - 1][j] > 0 && drp[i - 1][j] == waveCount + 1 - cnt)
							drp[i - 1][j] = -5;
						else if (j - 1 >= 0 && drp[i][j - 1] > 0 && drp[i][j - 1] == waveCount + 1 -cnt)
							drp[i][j - 1] = -5;
						else if (i + 1 < sizeDrpX && drp[i + 1][j] > 0 && drp[i + 1][j] == waveCount + 1 -cnt)
							drp[i + 1][j] = -5;
						else if (j + 1 < sizeDrpY && drp[i][j + 1] > 0 && drp[i][j + 1] == waveCount + 1 -cnt)
							drp[i][j + 1] = -5;
					}
				}
			}
		}
		//убираем значения волн, чтобы вывести только путь и две точки
		for (int i = 0; i < sizeDrpX; ++i)
		{
			for (int j = 0; j < sizeDrpY; ++j)
			{
				if (drp[i][j] > 0)
					drp[i][j] = 0;
			}
		}

		//снова рисуем в файле ДРП, но уже с проставленным путем и без нумерации волн
		for (int i = 0; i < sizeDrpX; ++i)
		{
			for (int j = 0; j < sizeDrpY; ++j)
			{
				switch (drp[i][j])
				{
				case 0: //пустая клетка (пол)
					file << std::setw(2) << '.' << " ";
					break;

				case -2: //стена
					file << std::setw(2) << '%' << " ";
					break;

				case -3: //точка А
					file << std::setw(2) << 'A' << " ";
					break;

				case -4: //точка В
					file << std::setw(2) << 'B' << " ";
					break;

				case -5://дорожка обратного пути
					file << std::setw(2) << '$' << " ";
					break;

				default:
					file << std::setw(2) << drp[i][j] << " ";
					break;
				}
			}
			file << std::endl;
		}
		
		//и снова выводим длинну пути
		if (lenght > waveCount)
			file << "\nPath lenght is " << lenght << " cells" << std::endl;
		if (waveCount >= lenght) //чтобы не багалось
			file << "\nPath lenght is " << waveCount + 1 << " cells" << std::endl;
		file.close();

		std::cout << "DRP was succsesfull print in file\n";

		break;

	case 2:
		return 0;
	}

}

int main(int argc, const char* argv[])
{
	//инициализация самой ДРП (-2 стенка, 0 пустота)
	int drp[sizeDrpX][sizeDrpY] =
	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, -2, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, -2, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	};

	//вывод дрп
	printDrp(drp);

	int aX = 1;
	int aY = 1;
	int bX = 2;
	int bY = 2;

		//запрашиваем у пользователя координаты точек А и В

	do //проверка, чтобы случайно не поставить точку А в стену
	{
		std::cout << "!!!Point cant be on a whall!!!\n";
		std::cout << "\nEnter the coordinates point A" << std::endl << "X - coordinates: ";
		aX = coordsPoint(aX);
		std::cout << "Y - coordinates: ";
		aY = coordsPoint(aY);
	} while (drp[aY - 1][aX - 1] == -2);

	do //проверка, чтобы у точек были различные координаты и чтобы В не поставить в стену
	{
		std::cout << "Points must have different coordinates\n";
		std::cout << "!!!Point cant be on a whall!!!\n";
		std::cout << "\n\nEnter the coordinates point B\nX - coordinates: ";
		bX = coordsPoint(bX);
		std::cout << "Y - coordinates: ";
		bY = coordsPoint(bY);
		std::cout << std::endl;
	} while ((aX == bX && aY == bY) || (drp[bY - 1][bX - 1]) == -2);

	std::swap(aX, aY);//ибо они не совсем корректно отображаются
	std::swap(bX, bY);


					  //записываем в файл получившуюся ДРП
	std::ofstream file;
	
	file.open("out.log");
	
	//вывод дрп с точками А и В
	if (changeDrp(drp, aX, aY, bX, bY) == 0)
	{
		for (int i = 0; i < sizeDrpX; ++i)
		{
			for (int j = 0; j < sizeDrpY; ++j)
			{
				switch (drp[i][j])
				{
				case 0: //пустая клетка (пол)
					file << std::setw(2) << '.' << " ";
					break;

				case -2: //стена
					file << std::setw(2) << '%' << " ";
					break;

				case -3: //точка А
					file << std::setw(2) << 'A' << " ";
					break;

				case -4: //точка В
					file << std::setw(2) << 'B' << " ";
					break;

				case -5://дорожка обратного пути
					file << std::setw(2) << '$' << " ";
					break;

				default:
					file << std::setw(2) << drp[i][j] << " ";
					break;
				}
			}
			file << std::endl;
		}
		std::cout<<std::endl;
		file << "Path not found or points are close to each other(Lenght path < 1)\n";
		return 0;
	}

	for (int i = 0; i < sizeDrpX; ++i)
	{
		for (int j = 0; j < sizeDrpY; ++j)
		{
			switch (drp[i][j])
			{
			case 0: //пустая клетка (пол)
				file << std::setw(2) << '.' << " ";
				break;

			case -2: //стена
				file << std::setw(2) << '%' << " ";
				break;

			case -3: //точка А
				file << std::setw(2) << 'A' << " ";
				break;

			case -4: //точка В
				file << std::setw(2) << 'B' << " ";
				break;

			case -5://дорожка обратного пути
				file << std::setw(2) << '$' << " ";
				break;

			default:
				file << std::setw(2) << drp[i][j] << " ";
				break;
			}
		}
			file << std::endl;
	}
		if (lenght > waveCount)
			file << "\nPath lenght is " << lenght << " cells" << std::endl;
		if (waveCount >= lenght) //чтобы не багалось
			file << "\nPath lenght is " << waveCount + 1 << " cells" << std::endl;

	file << "\n\n\n";//штоб не напутать потом


	printPath(drp, aX, aY, bX, bY, file);
	file.close();

	return 0;
	
}
