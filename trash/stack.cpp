#include <iostream>
#include <ctime>
#include <cassert>

class Node {
public:
    int value;
    Node *next;
};

class Stack : Node {
public:
    Node *head = NULL;
    int sizeOfStack = 0;

    void push(int value) {
        Node *Temp = new Node;
        Temp->value = value;
        Temp->next = head;
        head = Temp;
        ++sizeOfStack;
    }

    int pop() {
        if (sizeOfStack > 0) {
            int tempInt = head->value;
            Node *Temp = head;
            head = head->next;
            delete Temp;
            std::cout << tempInt << std::endl;
            --sizeOfStack;
            return tempInt;
        }
        else
            std::cout << "Stack is empty!" << std::endl;
    }

    void init(int initSize) {
        int temp = 0;
        for (int i = 0; i != initSize; ++i) {
             temp = rand() % 100;
             push(temp);
        }
    }

    void isEmpty() {
        if (!sizeOfStack)
            std::cout << "Stack is empty!" << std::endl;
        else
            std::cout << "Stack is not empty!" << std::endl;
    }

    void checkValue() {

        if (!sizeOfStack)
            std::cout << "Stack is empty!" << std::endl;
        else {
            Node *Temp = head;
            for(int i = 0; i != sizeOfStack; ++i) {
                int tempInt = Temp->value;
                Temp = Temp->next;
                std::cout << tempInt << std::endl;
            }
            delete Temp;
        }
    }
    void front() {
        std::cout << head->value << std::endl;
    }
    int &operator[] (int index);
    int tempInt = -1;
};

int &Stack::operator[] (int index) {
    assert(index < sizeOfStack);
    if (index < sizeOfStack) {
        Node *Temp = head;
        for(int i = 0; i < index; ++i) {
             Temp = Temp->next;
        }
        return Temp->value;
    }
    else {
        int error = -1;
        std::cout << "error " << std::endl;
        return error;
    }

}

int main()
{
    srand(time(NULL));
    Stack Example;
    int choice = 0, choiceInit = 0;
    std::cout << "Type any number to init a stack, 0 to continue\n";
    std::cin >> choiceInit;
    if (choiceInit) {
        std::cout << "Size?\n";
        int sizeOfStack;
        std::cin >> sizeOfStack;
        Example.init(sizeOfStack);
        std::cout << "Stack has been initialized!\n";
    }
    while(choice != 9) {
        std::cout << "Enter number:\n" <<
            "1 - push\n" <<
            "2 - pop\n" <<
            "3 - check value\n" <<
            "4 - check empty\n" <<
            "5 - check front\n" <<
            "6 - get value\n" <<
            "7 - set value\n" <<
            "9 - stop\n";
        std::cin >> choice;
        if (choice == 1) {
            std::cout << "Type any integer number: ";
            int number;
            std::cin >> number;
            Example.push(number);
        }
        if (choice == 2) {
            std::cout << "Last value was: ";
            Example.pop();
        }

        if (choice == 3) {
            Example.checkValue();
        }
        if (choice == 4) {
            Example.isEmpty();
        }
        if (choice == 5) {
            Example.front();
        }
        if (choice == 6) {
            int tempIndex;
            std::cout << "Type index" << std::endl;
            std::cin >> tempIndex;
            std::cout << Example[tempIndex] << std::endl;
        }
        if (choice == 7) {
            int tempIndex, tempValue;
            std::cout << "Type index" << std::endl;
            std::cin >> tempIndex;
            std::cout << "Type value" << std::endl;
            std::cin >> tempValue;
            Example[tempIndex] = tempValue;
        }
    }

    return 0;
}
