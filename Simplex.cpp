#include <iostream>
#include <sstream>
#include <iomanip>
#include "TaskClass.hpp"
#include "Simplex.hpp"

void DataSimplex::error(int num)
{
    switch (num) {
        case 0:
            std::cout << "Ошибка. Введенно некоректное значение" << std::endl;
            break;
        case 1:
            std::cout << "Решения не существует" << std::endl;
            break;
        case 2:
            std::cout << "ЦФ не ограничена" << std::endl;
            break;
    }
}

bool DataSimplex::getData(std::string fileName)
{ //метод заполнения симплекс таблицы
    if (!this->getDataFromFile(fileName)) { //считываем данные из файла
        return false;
    }

    tableCol = numberOfVariables + 2;
    tableRow = numberOfConstraints + 2;

    SimplexTable = new double*[tableRow]; //создаем симплекс таблицу с дополнительными строками
    for (int i = 0; i < tableRow; i++) //и столбцами (для записи номера переменных, свободных членов и ЦФ)
        SimplexTable[i] = new double[tableCol];

    for (int i = 2; i < tableCol; i++) //заполняем номера исходных переменных
        SimplexTable[0][i] = i - 1;

    for (int i = 1; i < tableRow - 1; i++) //заполняем номера дополнительных переменных
        SimplexTable[i][0] = numberOfVariables + i;

    for (int i = 1; i < tableRow - 1; i++) { //заполняем столбец свободных членов
        if (constraintSign[i - 1] == 3) {
            SimplexTable[i][1] = rightPart[i - 1] * (-1);
        }
        else {
            SimplexTable[i][1] = rightPart[i - 1];
        }
    }
    SimplexTable[tableRow - 1][1] = 0;

    for (int i = 2; i < tableCol; i++) { //заполняем последнюю строку коэффициентов ЦФ
        if (!functionWay) { //если ЦФ стремится к минимуму - нам нужно поменять знак
            SimplexTable[tableRow - 1][i] = objectiveFunction[i - 2] * (-1);
        } //если к максимуму - остается прежний, т.к. при вынесении минуса мы еще умножаем коэффициенты
        else { //на минус, чтобы привести функцию к min стремлению
            SimplexTable[tableRow - 1][i] = objectiveFunction[i - 2];
        }
    }

    for (int i = 1; i < tableRow - 1; i++) { //заполняем основную часть симплекс-таблицы
        for (int j = 2; j < tableCol; j++) {
            if (constraintSign[i - 1] == 3) { //если знак >=
                SimplexTable[i][j] = constraintSystem[i - 1][j - 2] * (-1);
            }
            else {
                SimplexTable[i][j] = constraintSystem[i - 1][j - 2];
            }
        }
    }

    return true;
}

void DataSimplex::print()
{
    std::cout << std::setw(15) << std::right << "\tSi\t";
    for (int i = 2; i < tableCol; i++)
        std::cout << std::setw(9) << std::right << "Y" << SimplexTable[0][i] << "\t";
    std::cout << std::endl;
    for (int i = 1; i < tableRow; i++) {
        for (int j = 0; j < tableCol; j++) {
            if ((j == 0) && (i != tableRow - 1))
                std::cout << "Y" << SimplexTable[i][j] << "\t";
            else if ((i == tableRow - 1) && (j == 0))
                std::cout << "F\t";
            else
                std::cout << std::setw(10) << std::right << SimplexTable[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}

bool DataSimplex::basicSolution()
{ //опорное решение
    while (negElementColumn(1)) {
        for (int i = 1; i < tableRow - 1; i++) { //идем по столбцу и ищем отрицательный
            if (SimplexTable[i][1] < 0) { //если найден отрицательный элемент
                if (negElementRow(i)) { //проверяем строку на наличие отрицательных
                    for (int j = 2; j < tableCol; j++) { //если есть хотя бы один отрицательный
                        if (SimplexTable[i][j] < 0) { //находим номер его столбца
                            iColumn = j; //и столбец становится разрешающим
                            int test = iColumn;
                            minRelation(test);
                            getTable();
                            print();
                            break;
                        }
                    }
                }
                else //если все элементы положительные
                {
                    return false; //переходим к следющему этапу (объявление отсутствия решения)
                }
                break;
            }
        }
    }

    std::cout << "Опорное решение найдено" << std::endl;
    return true; //опорное решение найдено
}

bool DataSimplex::optimalSolution()
{
    if (basicSolution()) //если найдено опорное решение
    { //проверяем есть ли в строке коэффициентов ЦФ положительный элемент
        while (posElements(tableRow - 1)) {
            for (int i = 2; i < tableCol; i++) { //находим значение его столбца
                if (SimplexTable[tableRow - 1][i] > 0) { //и проверяем есть ли в столбце положительный элемент
                    std::cout << "Положительный элемент в столбце " << i + 1 << std::endl;
                    if (noPosFunction(tableRow - 1)) {
                        iColumn = i;
                        int test = iColumn;
                        minRelation(test);
                        getTable();
                        print();
                        break;
                    }
                    else {
                        return false; //иначе решения не существует
                    }
                }
            }
        }
        if (negElementColumn(1)) //если в стобце свободных членов появляются отрицательные элементы
            return false;
        //если положительных в строке нет
        //то оптимальное решение найдено
        return true;
    }
    else //если оптимальное не найдено
    { //выводим ошибку и решения нет
        return false;
    }
}

void DataSimplex::getTable()
{
    std::cout << std::endl;
    double** NewTable;
    NewTable = new double*[tableRow];
    for (int i = 0; i < tableRow; i++)
        NewTable[i] = new double[tableCol];

    for (int i = 0; i < tableRow; i++)
        for (int j = 0; j < tableCol; j++)
            NewTable[i][j] = SimplexTable[i][j];

    std::swap(NewTable[iRow][0], NewTable[0][iColumn]);

    for (int i = 1; i < tableRow; i++)
        for (int j = 1; j < tableCol; j++)
            if ((i != iRow) && (j != iColumn))
                NewTable[i][j] -= (NewTable[iRow][j] * NewTable[i][iColumn]) / NewTable[iRow][iColumn];
    for (int i = 1; i < tableRow; i++)
        for (int j = 1; j < tableCol; j++)
            if ((i == iRow) && (j != iColumn))
                NewTable[i][j] /= NewTable[iRow][iColumn];

    for (int i = 1; i < tableRow; i++)
        for (int j = 1; j < tableCol; j++)
            if ((i != iRow) && (j == iColumn))
                NewTable[i][j] /= -1 * NewTable[iRow][iColumn];

    NewTable[iRow][iColumn] = 1 / SimplexTable[iRow][iColumn];

    SimplexTable = NewTable;
    std::cout << "Разрешающая строка: " << iRow + 1 << "\nРазрещающий столбец: " << iColumn + 1 << std::endl;
    std::cout << std::endl;
}

void DataSimplex::minRelation(int col)
{
    Tetha = new double[numberOfConstraints];
    double min;
    int j = 0;

    for (int i = 0; i < tableRow - 2; i++) {
        Tetha[i] = SimplexTable[i + 1][1] / SimplexTable[i + 1][col];
        if (Tetha[i] > 0) {
            min = Tetha[i];
            j = i;
        }
    }

    for (int i = 0; i < numberOfConstraints; i++) {
        if ((Tetha[i] > 0) && (Tetha[i] < min)) {
            min = Tetha[i];
            j = i;
        }
    }

    iRow = j + 1;

    delete[] Tetha;
}

bool DataSimplex::negElementRow(int row)
{ //проверка строки на наличие отрицательного элемента
    for (int i = 2; i < tableCol; i++)
        if (SimplexTable[row][i] < 0)
            return true;

    return false;
}

bool DataSimplex::posElements(int row)
{ //проверка строки на наличие положительного элемента
    for (int i = 2; i < tableCol; i++)
        if (SimplexTable[row][i] > 0)
            return true;

    return false;
}

bool DataSimplex::negElementColumn(int column)
{ //проверка на отрицательный элемент в столбце
    for (int i = 1; i < tableRow - 1; i++)
        if (SimplexTable[i][column] < 0)
            return true;

    return false;
}

bool DataSimplex::noPosFunction(int column)
{ //проверка столбца на наличие положительного элемента
    for (int i = 1; i < tableRow - 1; i++)
        if (SimplexTable[i][column] > 0)
            return true;

    return false;
}

void DataSimplex::printEnd()
{
    std::cout << "F = ";
    if (functionWay)
        std::cout << -1 * SimplexTable[tableRow - 1][1];
    else
        std::cout << SimplexTable[tableRow - 1][1];

    for (int i = 1; i < tableRow - 1; i++) {
        std::cout << " Y" << SimplexTable[i][0] << " = " << SimplexTable[i][1];
    }
    std::cout << std::endl;
}
