#include <iostream>
#include <string>
#include "Simplex.hpp"

using std::cout;
using std::endl;
using std::cin;

int main()
{
    cout << "Программа по решению двойственной задачи линейного программирования" << endl;
    cout << "Формат ввода данных в файл SimplexMethod.txt:\n"
         << "\tC = (0 0 0)\n\tA = (0 0 0\n\t     0 0 0\n\t     0 0 0)\n\t"
         << "B = (0 0 0)\n\tW = (max)\n\tS = (<= = >=)\n" << endl;

    DataSimplex myTable;
    cout.precision(4);
    cout << "Введите название файла, содержащего условия задачи: ";
    std::string nameOfFile; /* /Users/mac/CLionProjects/MOlab2/simplexmethod.txt */
    std::getline(cin, nameOfFile);

    if (myTable.getData(nameOfFile)) {
        cout << "\nИсходная симплекс таблица имеет вид:" << endl;
        myTable.print();

        if (myTable.optimalSolution()) {
            std::cout << "Оптимальное решение найдено: " << std::endl;
            myTable.printEnd();
        }
        else {
            myTable.print();
            myTable.error(1);
        }
    }
    else {
        cout << "Проверьте введенные данные и запустите программу снова" << endl;
    }

    cout << "ДО СВИДАНИЯ! :)" << endl;

    return 0;
}