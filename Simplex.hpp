
#ifndef MOLAB2_SIMPLEX_HPP
#define MOLAB2_SIMPLEX_HPP

#include <iostream>
#include "TaskClass.hpp"

class DataSimplex : public DataTask {
private:
    double** SimplexTable; //наша симплекс-таблица (изменяется на каждом этапе)
    double* Tetha; //отношение для коэффициентов
    int tableRow; //строк в таблице
    int tableCol; //столбцов в таблице
    int iColumn; //разрешающий стобец
    int iRow; //разрешающая строка
public:
    bool getData(std::string); //заполняем симплекс таблицу
    void getTable(); //функция жардановых исключений
    bool basicSolution(); //опорное решение
    bool optimalSolution(); //оптимальное решение
    void print(); //вывести симплекс таблицу
    void error(int num); //вывод ошибки
    bool negElementColumn(int); //проверка на наличие отрицательного элемента в столбце (для опорного решения)
    bool noPosFunction(int); //проверка на положительные элементы в столбце (для оптимального решения)
    bool negElementRow(int); //проверка на отрицательные элементы в строке (для опорного решения)
    bool posElements(int); //проверка на положительный в строке (для оптимального решения)
    void printEnd(); //решение
    void minRelation(int); //поиск минимального положительного отношения в двух столбцах
};

#endif //MOLAB2_SIMPLEX_HPP
