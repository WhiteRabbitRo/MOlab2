#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>

#include "TaskClass.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;

void error(int number)
{
    switch (number) {
        case 0:
            cout << "\nВы ввели неккоректные данные\n" << endl;
            break;
        case 1:
            cout << "\nФайл не найден\n" << endl;
            break;
    }
}

bool DataTask::getDataFromFile(string fileName)
{
    std::ifstream file;
    file.open(fileName, std::ios::in); //открываем файл для чтения данных

    std::vector<string> dataFromFile;
    string data; //переменная для строк
    if (file) { //если файл существует
        while (getline(file, data)) { //считываем строки, записываем их
            dataFromFile.push_back(data);
        }
    }
    else { //если не существует
        error(1); //выводим ошибку
        file.close();
        return false;
    }

    file.close();
    if (this->fillVariables(dataFromFile))
        return true;
    else
        return false;
}

bool DataTask::fillVariables(std::vector<string> fileData)
{
    std::vector<double> variablesVector; //для первоначальной записи данных
    string partOfLine; //считываем часть строки, которая может являться числом с плавающей точкой
    int j = 0;
    int counter = 0; //считает, какой размер должен быть у наших векторов
    for (int i = 0; i < fileData.size(); i++) { //идем по первому столбцу контейнера
        switch (fileData[i][0]) {
            case 'C': { //заполняем вектор правой части системы ограничений
                while (j != fileData[i].size()) { //идем до конца строки
                    if (fileData[i][j] == '(') { //если встречена открывающаяся скобка
                        j++;
                        while (fileData[i][j] != ')') { //пока не встречена закрывающаяся скобка
                            while (fileData[i][j] != ' ') { //пока не встречен пробел
                                partOfLine += fileData[i][j]; //считываем значение
                                j++;
                                if (fileData[i][j] == ')') {
                                    j--;
                                    break;
                                }
                            }

                            if ((atof(partOfLine.c_str()) == 0) && (partOfLine != "0")) { //если есть не-цифры
                                error(0);
                                return false;
                            }

                            variablesVector.push_back(atof(partOfLine.c_str())); //заносим значение в контейнер
                            partOfLine.clear(); //отчищаем строку
                            counter++; //увеличиваем счетчик чисел
                            j++;
                        }
                    }
                    j++;
                }

                rightPart = new double[counter];
                for (j = 0; j < variablesVector.size(); j++) {
                    rightPart[j] = variablesVector[j]; //заполняем вектор правой части системы ограничений
                }

                numberOfConstraints = counter; //число ограничений
                counter = 0;
                j = 0;
                variablesVector.clear();
            } break;
            case 'A': {
                int numberOfRows = 1;
                int Row = i;

                while (fileData[Row][j - 1] != '(') { //ищем открывающуюся скобку
                    j++;
                }

                while (fileData[Row][j] != ')') {
                    while (fileData[Row][j] != ' ') { //пока не встречен пробел
                        partOfLine += fileData[Row][j]; //считываем значение
                        j++;
                        if (j == fileData[Row].size()) {
                            counter = 0;
                            numberOfRows++; //считаем количество строк(ограничений)
                            Row++;
                            j = -1;
                            break;
                        }

                        if (fileData[Row][j] == ')') {
                            j--;
                            break;
                        }
                    }

                    if ((atof(partOfLine.c_str()) == 0) && (partOfLine != "0")) { //если есть не-цифры
                        error(0);
                        return false;
                    }

                    variablesVector.push_back(atof(partOfLine.c_str())); //заносим значение в контейнер
                    partOfLine.clear(); //отчищаем строку
                    counter++; //увеличиваем счетчик чисел
                    j++;
                }

                counter--;
                constraintSystem = new double*[counter];
                for (int k = 0; k < counter; k++)
                    constraintSystem[k] = new double[numberOfRows];

                Row = 0;
                for (int k = 0; k < numberOfRows; k++) //заполняем матрицу системы ограничений
                    for (int l = 0; l < counter; l++) {
                        constraintSystem[l][k] = variablesVector[Row];
                        Row++;
                    }

                counter = 0;
                j = 0;
                variablesVector.clear();
            } break;
            case 'B': { //заполняем вектор коэффициентов
                while (j != fileData[i].size()) { //идем до конца строки
                    if (fileData[i][j] == '(') { //если встречена открывающаяся скобка
                        j++;
                        while (fileData[i][j] != ')') { //пока не встречена закрывающаяся скобка
                            while (fileData[i][j] != ' ') { //пока не встречен пробел
                                partOfLine += fileData[i][j]; //считываем значение
                                j++;
                                if (fileData[i][j] == ')') {
                                    j--;
                                    break;
                                }
                            }
                            if ((atof(partOfLine.c_str()) == 0) && (partOfLine != "0")) { //если есть не-цифры
                                error(0);
                                return false;
                            }
                            variablesVector.push_back(atof(partOfLine.c_str())); //заносим значение в контейнер
                            partOfLine.clear(); //отчищаем строку
                            counter++; //увеличиваем счетчик чисел
                            j++;
                        }
                    }
                    j++;
                }

                objectiveFunction = new double[counter];
                for (j = 0; j < variablesVector.size(); j++) {
                    objectiveFunction[j] = variablesVector[j]; //заполняем вектор правой части системы ограничений
                }

                numberOfVariables = counter; //число переменных
                counter = 0;
                j = 0;
                variablesVector.clear();
            } break;
            case 'W': { //заполняем направление ЦФ
                while (j != fileData[i].size()) { //идем до конца строки
                    if (fileData[i][j] == '(') { //если встречена открывающаяся скобка
                        j++;
                        while (fileData[i][j] != ')') { //пока не встречена закрывающаяся скобка
                            while (fileData[i][j] != ' ') { //пока не встречен пробел
                                partOfLine += fileData[i][j]; //считываем значение
                                j++;
                                if (fileData[i][j] == ')') {
                                    j--;
                                    break;
                                }
                            }

                            if (partOfLine == "min")
                                functionWay = true;
                            else if (partOfLine == "max")
                                functionWay = false;
                            else
                                cout << "Неверно введено направление ЦФ" << endl;

                            partOfLine.clear(); //отчищаем строку
                            j++;
                        }
                    }
                    j++;
                }

                j = 0;
            } break;
            case 'S': {
                std::vector<int> signVector;
                while (j != fileData[i].size()) { //идем до конца строки
                    if (fileData[i][j] == '(') { //если встречена открывающаяся скобка
                        j++;
                        while (fileData[i][j] != ')') { //пока не встречена закрывающаяся скобка
                            while (fileData[i][j] != ' ') { //пока не встречен пробел
                                partOfLine += fileData[i][j]; //считываем значение
                                j++;
                                if (fileData[i][j] == ')') {
                                    j--;
                                    break;
                                }
                            }

                            //заполняем контейнер в зависимости от знака
                            if (partOfLine == "=")
                                signVector.push_back(1);
                            else if (partOfLine == "<=")
                                signVector.push_back(3);
                            else if (partOfLine == ">=")
                                signVector.push_back(2);
                            else
                                cout << "Неверно введен знак системы ограничений" << endl;

                            partOfLine.clear(); //отчищаем строку
                            counter++; //увеличиваем счетчик чисел
                            j++;
                        }
                    }
                    j++;
                }

                constraintSign = new int[counter];
                for (j = 0; j < signVector.size(); j++) {
                    constraintSign[j] = signVector[j]; //заполняем знаки системы ограничений
                }

                counter = 0;
                j = 0;
                signVector.clear();
            } break;
            default: {

            } break;
        }
    }
    return true;
}