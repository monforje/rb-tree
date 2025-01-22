#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <iostream>

// lNode structure for the doubly linked list
struct lNode {
    int data;
    lNode* next;
    lNode* prev;

    lNode(int value) : data(value), next(nullptr), prev(nullptr) {}
};

class DoublyLinkedList {
private:
    lNode* head;
    lNode* tail;

public:
    // Constructor
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    // Destructor
    ~DoublyLinkedList() {
        lNode* current = head;
        while (current) {
            lNode* nextlNode = current->next;
            delete current;
            current = nextlNode;
        }
    }

    // Push an element to the back of the list
    void push_back(int value) {
        lNode* newlNode = new lNode(value);
        if (!head) {
            head = tail = newlNode;
        } else {
            tail->next = newlNode;
            newlNode->prev = tail;
            tail = newlNode;
        }
    }

    // Print the list elements
    void print() const {
        lNode* current = head;
        std::cout << "[";
        while (current) {
            std::cout << current->data;
            if (current->next) {
                std::cout << ", "; // Добавить запятую между элементами
            }
            current = current->next;
        }
        std::cout << "]"; // Закрыть скобку и добавить перенос строки
    }

    // Delete an element with the given value
    void delete_value(int value) {
        lNode* current = head;
        while (current) {
            if (current->data == value) {
                if (current->prev) {
                    current->prev->next = current->next;
                } else {
                    head = current->next;
                }

                if (current->next) {
                    current->next->prev = current->prev;
                } else {
                    tail = current->prev;
                }

                delete current;
                return;
            }
            current = current->next;
        }
    }
};

#endif // DOUBLY_LINKED_LIST_H