//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_LIST_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_LIST_HPP
#include "../test/printing.hpp"

template<typename T>
class List
{
private:
    struct Elem
    {
        T *data;
        Elem *next;

        Elem(T *data, Elem *next) : data(data), next(next) {}
    };

    Elem *head = nullptr, *tail = nullptr;

    //int help = 0;

    static int num;

public:
    List() : head(0), tail(0) {}

    List(const List<T> &) = delete;

    List<T> &operator=(const List<T> &) = delete;

    Elem * getHead(){
        return head;
    }
    Elem * getTail(){
        return tail;
    }


    void addFirst(T *data)
    {
        Elem *elem = new Elem(data, head);
        head = elem;
        num++;
        if (!tail) { tail = head; }
    }

    void setEmpty(){
        tail = nullptr;
        head = nullptr;
        //num = 0;
    }

    void addLast(T *data)
    {
        Elem *elem = new Elem(data, 0);
        num++;

        if (tail)
        {
            tail->next = elem;
            tail = elem;
        } else
        {
            head = tail = elem;
        }
    }

    T *removeFirst()
    {
        //printString("Uslo se u remove list\n");
        if (!head) { return 0; }

        num--;

        //printInt(num);
        Elem *elem = head;
        head = head->next;
        if (!head) { tail = 0; }

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T *peekFirst()
    {
        if (!head) { return 0; }
        return head->data;
    }

    T *removeLast()
    {
        if (!head) { return 0; }

        Elem *prev = 0;
        for (Elem *curr = head; curr && curr != tail; curr = curr->next)
        {
            prev = curr;
        }

        Elem *elem = tail;
        if (prev) { prev->next = 0; }
        else { head = 0; }
        tail = prev;

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T *peekLast()
    {
        if (!tail) { return 0; }
        return tail->data;
    }
    /*
    T* printList() {
        Elem *current = head;
        if(current == nullptr){
            printString("Empty queue of blocked threads!\n");
        }
        printString("\nNumber of blocked: ");
        printInt(num);
        printString("->");
        while (current) {
            printString("TCB ID: ");
            printInt(current->data);
            current = current->next;
            if(current) printString(" -> ");
            else printString("\n");
        }
        return nullptr;
    }*/
};
template<typename T>
int List<T>::num = 0;
#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_LIST_HPP
