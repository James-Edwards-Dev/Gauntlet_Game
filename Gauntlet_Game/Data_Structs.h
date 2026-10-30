#pragma once
#include <iostream>

template<typename T>
struct node
{
    T data;
    node* next;
    
    node(const T d_) : data(d_), next(nullptr) {}
};

template<typename T>
class Stack 
{
    node<T>* top;
public:
    Stack() : top(nullptr) {}
    
    ~Stack() {
        while (top != nullptr) {
            node<T>* n = top;
            top = top->next;
            delete n;
        }
    }
    void push(T d) {
        node<T>* n = new node<T>(d);
        n->next =  top;
        top = n;
    }
    bool pop(T& d) {
        if (top == nullptr) return false;
        
        node<T>* n = top;
        d = top->data;
        top = top->next;
        delete n;
        
        return true;
    }
    void display() const 
    {
        std::cout << "T->";
        for (node<T>* n = top; n != nullptr; n = n->next) {
            std::cout << n->data << "\t";
        }
        std::cout << "\n";
    }
};