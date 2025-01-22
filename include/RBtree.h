#ifndef RBTREE_H
#define RBTREE_H

#include<iostream>
#include"DoublyLinkedList.h"
#include<fstream>
#include<sstream>

// Паспорт
struct Passport {
    int series;     // серия
    int passport;   // номер
    int line;
    Passport() : series(0), passport(0), line(0) {} // Default constructor
    Passport(int s, int p) : series(s), passport(p), line(0) {} // Parameterized constructor
};

enum Color { RED, BLACK };  

// Узел дерева
struct Node {
    Passport passport;
    Color color;
    Node* parent;
    Node* right;
    Node* left;
    DoublyLinkedList DuplicateList;

    Node(Passport k) : passport(k), color(BLACK), parent(nullptr), right(nullptr), left(nullptr), DuplicateList() {}; 
};

class RBtree {
private:
    // Корень дерева
    Node* root;
    // Поворот налево
    void leftRotate(Node*& root, Node* x) {
        // Указатель на правого потомка x
        Node* y = x->right;

        // Перемещаем левое поддерево y на место правого поддерева x
        x->right = y->left;
        if (y->left != nullptr) {
            y->left->parent = x;
        }

        // Устанавливаем родителя y в качестве родителя x
        y->parent = x->parent;
        if (x->parent == nullptr) {
            // x был корнем, теперь y становится корнем
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }

        // Делаем x левым потомком y
        y->left = x;
        x->parent = y;
    }
    // Поворот направо
    void rightRotate(Node*& root, Node* y) {
        // Указатель на левого потомка y
        Node* x = y->left;

        // Перемещаем правое поддерево x на место левого поддерева y
        y->left = x->right;
        if (x->right != nullptr) {
            x->right->parent = y;
        }

        // Устанавливаем родителя x в качестве родителя y
        x->parent = y->parent;
        if (y->parent == nullptr) {
            // y был корнем, теперь x становится корнем
            root = x;
        } else if (y == y->parent->right) {
            y->parent->right = x;
        } else {
            y->parent->left = x;
        }

        // Делаем y правым потомком x
        x->right = y;   
        y->parent = x;
    }
    // Исправить нарушения после вставки узла
    void fixInsert(Node*& root, Node* z) {
        // Исправляет нарушения красно-черного дерева после вставки узла z
        while (z != root && z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right; // Дядя узла z
                if (y != nullptr && y->color == RED) {
                    // Случай 1: Дядя красный
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        // Случай 2: z — правый потомок
                        z = z->parent;
                        leftRotate(root, z);
                    }
                    // Случай 3: z — левый потомок
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(root, z->parent->parent);
                }
            } else {
                Node* y = z->parent->parent->left; // Дядя узла z
                if (y != nullptr && y->color == RED) {
                    // Случай 1: Дядя красный
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        // Случай 2: z — левый потомок
                        z = z->parent;
                        rightRotate(root, z);
                    }
                    // Случай 3: z — правый потомок
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(root, z->parent->parent);
                }
            }
        }
        root->color = BLACK; // Корень всегда черный
    }

    // Функция transplant, используемая при удалении
    void transplant(Node*& root, Node* u, Node* v) {
        // Заменяет поддерево с корнем в u поддеревом с корнем в v
        if (u->parent == nullptr) {
            root = v; // Если u — корень, то v становится новым корнем
        } else if (u == u->parent->left) {
            u->parent->left = v; // u был левым потомком
        } else {
            u->parent->right = v; // u был правым потомком
        }
        if (v != nullptr) {
            v->parent = u->parent; // Устанавливаем родителя v
        }
    }

    // Найти максимальный слева узел в поддереве
    Node* maximum(Node* node) {
        while (node->right != nullptr) node = node->right;
        return node;
    }
    
    // Удаление node
    void deleteNode(Node*& root, Node* z) {
        // Удаляет узел z из красно-черного дерева
        Node* y = z;
        Node* x;
        Node* k = z;
        Color originalColor = y->color;
        if (z->left == nullptr) {
            x = z->right;
            transplant(root, z, z->right);
        } else if (z->right == nullptr) {
            x = z->left;
            transplant(root, z, z->left);
        } else {
            y = maximum(z->left); // Предшественник z
            originalColor = y->color;
            x = y;
            if (y == z) {
                if (x != nullptr) {
                    x->parent = y;
                }
            } else {
                transplant(root, y, y->left);
                y->left = z->left;
                y->left->parent = y;
            }
            transplant(root, z, y);
            y->right = z->right;
            y->right->parent = y;
            y->color = z->color;
        }

        if (originalColor == BLACK && x != nullptr && x != root) {
            fixDelete(root, x);
        }
    }

    // Функция исправления нарушений после удаления узла
    void fixDelete(Node*& root, Node* x) {
        // Исправляет нарушения красно-черного дерева после удаления узла x
        while (x != root && (x == nullptr || x->color == BLACK)) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(root, x->parent);
                    w = x->parent->right;
                }
                if ((w->left == nullptr || w->left->color == BLACK) &&
                    (w->right == nullptr || w->right->color == BLACK)) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right == nullptr || w->right->color == BLACK) {
                        if (w->left != nullptr) {
                            w->left->color = BLACK;
                        }
                        w->color = RED;
                        rightRotate(root, w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->right != nullptr) {
                        w->right->color = BLACK;
                    }
                    leftRotate(root, x->parent);
                    x = root;
                }
            } else {
                Node* w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(root, x->parent);
                    w = x->parent->left;
                }
                if ((w->right == nullptr || w->right->color == BLACK) &&
                    (w->left == nullptr || w->left->color == BLACK)) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left == nullptr || w->left->color == BLACK) {
                        if (w->right != nullptr) {
                            w->right->color = BLACK;
                        }
                        w->color = RED;
                        leftRotate(root, w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->left != nullptr) {
                        w->left->color = BLACK;
                    }
                    rightRotate(root, x->parent);
                    x = root;
                }
            }
        }
        if (x != nullptr) {
            x->color = BLACK;
        }
    }
    // Распечатать древовидную структуру (прямой обход)
    void printHelper(Node* root, int space) {
        // Печатает дерево в древовидной форме (прямой обход)
        if (root == nullptr) {
            return;
        }
        const int COUNT = 10;
        space += COUNT;
        printHelper(root->right, space);
        std::cout << std::endl;
        for (int i = COUNT; i < space; i++) {
            std::cout << " ";
        }
        std::cout << root->passport.series << " " << root->passport.passport << (root->color == RED ? "(R)" : "(B)") << "\n";
        printHelper(root->left, space);
    }
    // Распечатать прямой обход
    void print_pre_order(Node* root) {

        if (root == nullptr) return;
        Node* current = root;
        std::cout << root->passport.series << " " << root->passport.passport << (root->color == RED ? "(R)" : "(B)") << ": "; 
        root->DuplicateList.print();
        std::cout << std::endl;
        print_pre_order(root->left);
        print_pre_order(root->right);
    }
    // Дополнительно
    void exportToGraphviz(Node* root, std::ofstream& out) {
        if (root == nullptr) {
            return;
        }

        // Записываем текущий узел
        out << "\t\"" << root->passport.series << "_" << root->passport.passport << "\" [label=\""
            << root->passport.series << " " << root->passport.passport << "\n";
        out << (root->color == RED ? "RED" : "BLACK") << "\"]";
        out << ";\n";

        // Если есть левый потомок, соединяем с ним
        if (root->left != nullptr) {
            out << "\t\"" << root->passport.series << "_" << root->passport.passport << "\" -> \""
                << root->left->passport.series << "_" << root->left->passport.passport << "\";\n";
        }

        // Если есть правый потомок, соединяем с ним
        if (root->right != nullptr) {
            out << "\t\"" << root->passport.series << "_" << root->passport.passport << "\" -> \""
                << root->right->passport.series << "_" << root->right->passport.passport << "\";\n";
        }

        // Рекурсивно обрабатываем потомков
        exportToGraphviz(root->left, out);
        exportToGraphviz(root->right, out);
    }
    // Поиск заданного элемента (if node in tree = true/false)
    bool searchTreeNode(int series, int passport) {
        Node* current = root; // Начинаем поиск с корня
        if (current == nullptr) {
            std::cout << "Tree is empty!" << std::endl;
            return false; // Explicit return when the tree is empty
        }

        while (current != nullptr) {
            if (current->passport.series == series && current->passport.passport == passport) {
                std::cout << series << " " << passport << " is in the Tree !!!" << std::endl;
                return true; // Элемент найден
            } else if (series < current->passport.series || 
                    (series == current->passport.series && passport < current->passport.passport)) {
                current = current->left; // Идем в левое поддерево
            } else {
                current = current->right; // Идем в правое поддерево
            }
        }

        std::cout << series << " " << passport << " is not in the Tree !!!" << std::endl; // Элемент не найден
        return false; // Explicit return when the element is not found
    }

    // Рекурсивная функция для удаления узлов
    void deleteTree(Node* node) {
        if (node == nullptr) {
            return;
        }
        deleteTree(node->left);  // Удаляем левое поддерево
        deleteTree(node->right); // Удаляем правое поддерево
        delete node;             // Удаляем текущий узел
    }

public:
    RBtree() : root(nullptr) {}
    // Вставить пасспорт
    void insert(Passport p) {
        Passport newPassport = {p.series, p.passport};
        newPassport.line = p.line;

        // Создать новый узел для вставки
        Node* newNode = new Node(newPassport);
        newNode->color = RED; // Новый узел всегда красный
        newNode->left = newNode->right = nullptr;

        // Если дерево пустое, делаем новый узел корнем
        if (root == nullptr) {
            newNode->color = BLACK; // Корень всегда черный
            root = newNode;
            return;
        }
        
        // Найти подходящее место для вставки
        Node* current = root;
        Node* parent = nullptr;
        while (current != nullptr) {
            parent = current;
            if (newPassport.series == current->passport.series && newPassport.passport == current->passport.passport) {
                // Если нашли дубликат, добавляем в список дубликатов и выходим
                current->DuplicateList.push_back(newPassport.line);
                delete newNode; // Удаляем новый узел, так как он не нужен
                return;
            } else if (newPassport.series < current->passport.series ||
                    (newPassport.series == current->passport.series && newPassport.passport < current->passport.passport)) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        // Устанавливаем родителя нового узла
        newNode->parent = parent;
        if (newPassport.series < parent->passport.series ||
            (newPassport.series == parent->passport.series && newPassport.passport < parent->passport.passport)) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }

        // Исправляем нарушения красно-черного дерева
        fixInsert(root, newNode);
    }
    // Удалить паспорт
    void remove(Passport p) {
        int series = p.series;
        int passport = p.passport;
        Node* temp = root;
        while (temp != nullptr) {
            if (temp->passport.series == series && temp->passport.passport == passport) {
                deleteNode(root, temp);
                return;
            } else if (series < temp->passport.series || (series == temp->passport.series && passport < temp->passport.passport)) {
                temp = temp->left;
            } else {
                temp = temp->right;
            }
        }
    }
    // Напечатать в древовидном виде
    void printTree() {
        printHelper(root, 0);
    }
    // Есть ли?
    bool searchTreeNode(const Passport& p) {
        return searchTreeNode(p.series, p.passport);
    }
    // Вывод в прямом обходе
    void printPreOrder() {
        print_pre_order(root);
        std::cout << std::endl;
    }
    // Вставка из файла
    void insertFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Unable to open file: " << filename << std::endl;
            return;
        }

        std::string line;
        int lineNumber = 0;
        while (std::getline(file, line)) {
            lineNumber++;
            std::istringstream iss(line);
            int series, passport;
            if (!(iss >> series >> passport)) {
                std::cerr << "Invalid data on line " << lineNumber << ": " << line << std::endl;
                continue;
            }
            Passport p = {series, passport};
            p.line = lineNumber;
            insert(p);
        }

        file.close();
    }
    // Дополнительно : dot -Tpng tree.dot -o tree.png
    void exportToGraphviz(const std::string& filename) {
        std::ofstream out(filename);
        if (!out.is_open()) {
            std::cerr << "Unable to open file: " << filename << std::endl;
            return;
        }

        out << "digraph RBTree {\n";
        out << "\tnode [fontname=\"Arial\", shape=circle];\n";

        if (root == nullptr) {
            out << "\tEmptyTree;\n";
        } else {
            exportToGraphviz(root, out);
        }

        out << "}";
        out.close();
    }

    // Деструктор
    ~RBtree() {
        deleteTree(root); // Рекурсивно удаляем все узлы
    }
};

#endif // RBTREE_H