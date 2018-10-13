#ifndef MOLAB2_TASKCLASS_HPP
#define MOLAB2_TASKCLASS_HPP

#include <iostream>
#include <vector>

class DataTask {
protected:
    double* objectiveFunction; //коэффициенты ЦФ (правая часть в ПЗ)
    double* rightPart; //правая часть системы (коэффициенты ЦФ в ПЗ)
    double** constraintSystem; //коэффициенты системы ограничений (транспонированная матрица ПЗ)
    int* constraintSign; //знаки системы ограничений (1 - =, 3 - <=, 2 - >=)
    bool functionWay; //направление функции (false - min, true - max)
    int numberOfVariables; //число переменных (число ограничений в ПЗ)
    int numberOfConstraints; //число ограничений (число переменных в ПЗ)
public:
    bool getDataFromFile(std::string); //метод получения данных задачи из файла
    bool fillVariables(std::vector<std::string>); //метод заполнения данных в поля класса
};

#endif //MOLAB2_TASKCLASS_HPP
