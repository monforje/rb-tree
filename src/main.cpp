#include "../include/RBtree.h"
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

void menu() {
    cout << "\n==============================\n";
    cout << "            МЕНЮ              \n";
    cout << "==============================\n";
    cout << left << setw(30) << " 1. Вставить паспорт" << endl;
    cout << left << setw(30) << " 2. Удалить паспорт" << endl;
    cout << left << setw(30) << " 3. Поиск паспорта" << endl;
    cout << left << setw(30) << " 4. Вывести дерево" << endl;
    cout << left << setw(30) << " 5. Прямой обход" << endl;
    cout << left << setw(30) << " 6. Нарисовать дерево" << endl;
    cout << left << setw(30) << " 7. Выход" << endl;
    cout << "==============================\n";
    cout << "Выберите пункт: ";
}

int main() {
    RBtree tree;
    string filename = "../DATA.txt";
    tree.insertFromFile(filename); // Вставляем данные из файла
    // Passport p;
    // Passport p1;
    // p.series = 1234;
    // p.passport = 567890;
    // p.line = 1;
    // p1.series = 1234;
    // p1.passport = 567890;
    // p1.line = 2;
    // tree.insert(p);
    // tree.insert(p1);
    while (true) {
        menu();
        int choice;
        cin >> choice;
        switch (choice) {
            case 1: {
                int series, number;
                cout << "Введите серию паспорта: ";
                cin >> series;
                cout << "Введите номер паспорта: ";
                cin >> number;
                Passport p(series, number);
                tree.insert(p);
                cout << "Паспорт добавлен.\n";
                break;
            }
            case 2: {
                int series, number;
                cout << "Введите серию паспорта для удаления: ";
                cin >> series;
                cout << "Введите номер паспорта для удаления: ";
                cin >> number;
                Passport p(series, number);
                tree.remove(p);
                cout << "Паспорт удалён (если существовал).\n";
                break;
            }
            case 3: {
                int series, number;
                cout << "Введите серию паспорта для поиска: ";
                cin >> series;
                cout << "Введите номер паспорта для поиска: ";
                cin >> number;
                Passport p(series, number);
                if (tree.searchTreeNode(p)) {
                    cout << "Паспорт найден в дереве.\n";
                } else {
                    cout << "Паспорт не найден.\n";
                }
                break;
            }
            case 4: {
                cout << "Содержимое дерева:\n";
                tree.printTree();
                break;
            }
            case 5: {
                cout << "Прямой обход дерева:\n";
                tree.printPreOrder();
                break;
            }
            case 6: {
                string graphvizFile = "tree.dot";
                tree.exportToGraphviz(graphvizFile);
                cout << "Дерево экспортировано в файл " << graphvizFile << ".\n";
                cout << "Введите: dot -Tpng tree.dot -o tree.png" << endl;
                break;
            }
            case 7: {
                cout << "Выход из программы.\n";
                return 0;
            }
            default: {
                cout << "Неверный выбор. Попробуйте ещё раз.\n";
                break;
            }
        }
    }

    return 0;
}