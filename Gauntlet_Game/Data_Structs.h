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
struct Stack 
{
    node<T>* top;
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

// Helper function to delete all data from each node in the stack.
// Needs to be used when deleting a stack of pointers, to prevent leaking memory.
template<typename T>
void clear_stack(Stack<T>& stack) 
{
    while (stack.top != nullptr) {
        node<T>* n = stack.top;
        stack.top = stack.top->next;
        delete n->data;
        delete n;
    }
}