#include <iostream>
#include <fstream>
#include <string>

const int sizeDrpX = 20;
const int sizeDrpY = 20;

void printDrp(int drp[sizeDrpX][sizeDrpY]) //начальная ДРП, из файла
{
	std::cout << "'" << (char)95 << "'" << " - floor \n";
	std::cout << "'" << (char)35 << "'" << " - wall\n\n";
	for (int i = 0; i < sizeDrpX; ++i)
	{
		for (int j = 0; j < sizeDrpY; ++j)
		{
			if (drp[i][j] == 0)
				std::cout << '_' << " ";
			else if (drp[i][j] == -2)
				std::cout << '#' << " ";
			else if (drp[i][j] == -3)
				std::cout << 'A' << " ";
			else if (drp[i][j] == -4)
				std::cout << 'B' << " ";
			else
				std::cout << drp[i][j] << " ";
		}
		std::cout << "\n";
	}
}

//функция вывода ДРП, но уже с расставленными точками и путем
void printDrp(int drp[sizeDrpX][sizeDrpY], int aX, int aY, int bX, int bY)
{
	int waveCount = 0; //счетчик волн
	bool hasChanged = true; 

	drp[aX - 1][aY - 1] = -3; // Точка А имеет значение -3
	drp[bX - 1][bY - 1] = -4; // Точка Б имеет значение -4

	while (hasChanged) 
	{ 
		hasChanged = false;
		//перебираем все элементы ДРП
		for (int i = 1; i < sizeDrpX; ++i) 
		{
			for (int j = 0; j < sizeDrpY; ++j) 
			{
				if (drp[i][j] != 0) 
				{ // проверяем соседей для каждого ненулевого элемента
					if (i + 1 < sizeDrpX && drp[i + 1][j] == 0) 
					{
						drp[i + 1][j] = waveCount + 1;
						hasChanged = true;
					}
					if (j + 1 < sizeDrpY && drp[i][j + 1] == 0) 
					{
						drp[i][j + 1] = waveCount + 1;
						hasChanged = true;
					}
					if (i - 1 >= 0 && drp[i - 1][j] == 0) 
					{
						drp[i - 1][j] = waveCount + 1;
						hasChanged = true;
					}
					if (j - 1 >= 0 && drp[i][j - 1] == 0) 
					{
						drp[i][j - 1] = waveCount + 1;
						hasChanged = true;
					}
				}
			}
		}

		//инкрементируем волну
		waveCount++;
	}

	//печатаем конечную ДРП
	std::cout<<std::endl;
	printDrp(drp);

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