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
struct doubly_node
{
    T data;
    doubly_node* next; 
    doubly_node* previous;
    
    doubly_node(const T d_) : data(d_), next(nullptr), previous(nullptr) {}
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

template<typename T>
struct doubly_linked_list
{
    doubly_node<T>* head; // Pointer to first node in the list
    doubly_node<T>* tail; // Pointer to last node in the list
    unsigned int size = 0;
    doubly_linked_list() : head(nullptr), tail(nullptr){}
    
    ~doubly_linked_list() {
        while (head != nullptr) {
            doubly_node<T>* n = head;
            head = head->next;
            delete n;
        }
    }
    
    void append(T data) {
        doubly_node<T>* n =  new doubly_node<T>(data);
        
        if (tail != nullptr) {
            tail->next = n;
            n->previous = tail;
        } else {
            head = n;
        }
        tail = n;
        size++;
    }
    T& operator [](int index)
    {
        doubly_node<T>* current = head;
        for (unsigned int i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }
    doubly_node<T>* find(T d) {
        for (doubly_node<T>* n = head; n != nullptr; n = n->next) {
            if (n->data == d) {
                return n;
            }
        }
        return nullptr;
    }
    void remove(doubly_node<T>* node) {
        if (node == nullptr) return;
        
        if (node->previous != nullptr) {
            node->previous->next = node->next;
        } else {
            head = node->next;
        }
        
        if (node->next != nullptr) {
            node->next->previous = node->previous;
        } else {
            tail = node->previous;
        }

        size--;
        delete node;
    }
    void display() const 
    {
        std::cout << "H->";
        for (doubly_node<T>* n = head; n != nullptr; n = n->next) {
            std::cout << n->data;
            if (n != tail) {
                std::cout << "\t";
            }
        }
        std::cout << "<-T\n";
    }
    // node<T>* n = new node<T>(d);
    // n->next =  top;
    // top = n;
};