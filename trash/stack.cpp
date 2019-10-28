#include <iostream>
#include <ctime>

class Node {
public:
    int value;
    Node *next;
};

class Stack : public Node {
protected:
    Node *head = NULL;
    int sizeOfStack = 0;

    void push(int value) {
        Node *Temp = new Node;
        Temp->value = value;
        Temp->next = head;
        head = Temp;
        ++sizeOfStack;
    }

    void pop() {
        if (sizeOfStack > 0) {
            Node *Temp = head;
            head = head->next;
            delete Temp;
            --sizeOfStack;
        }
        else
            std::cout << "Stack is empty!" << std::endl;
    }

    void init(int initSize) {
        for (int i = 0; i != initSize; ++i) {
             push(rand() % 100);
        }
    }

    void isEmpty() {
        if (!sizeOfStack)
            std::cout << "Stack is empty!" << std::endl;
        else
            std::cout << "Stack is not empty!" << std::endl;
    }

    void checkValues() {
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
            /*
            int *tempArr = new int[sizeOfStack];
            int TempSize = sizeOfStack;
            for(int i = TempSize; i != 0; --i) {
            	tempArr[i] = head->value;
            	pop();
            }
            for(int i = 0; i != TempSize; ++i) {
            	std::cout << tempArr[i] << std::endl;
            	push(tempArr[i]);
            }
            delete tempArr;*/
        }
    }
    void front() {
    	if(!sizeOfStack)
    	    std::cout << "Stack is empty!" << std::endl;
    	else
            std::cout << head->value << std::endl;
    }
};


class StackD : public Stack {
public:
    void pushD(int number) { push(number); }
    void popD() { pop(); }
    void initD(int sizeOfStack) { init(sizeOfStack); }
    void isEmptyD() { isEmpty(); }
    void checkValuesD() { checkValues(); }
    void frontD() { front(); }
    int size() { return sizeOfStack; }

    int &operator[] (int index);
    int tempInt = -1;
};

int &StackD::operator[] (int index) {
    Node *Temp = head;
    for(int i = 0; i < index; ++i) {
         Temp = Temp->next;
    }
    return Temp->value;
}

int main()
{
    srand(time(NULL));
    StackD Example;
    int choice = 0, choiceInit = 0;
    std::cout << "Type any number to init a stack, 0 to continue\n";
    std::cin >> choiceInit;
    if (choiceInit) {
        std::cout << "Size?\n";
        int sizeOfStack;
        std::cin >> sizeOfStack;
        Example.initD(sizeOfStack);
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
            "9 - stop\n" <<
            "> ";
        std::cin >> choice;
        if (choice == 1) {
            std::cout << "Type any integer number: ";
            int number;
            std::cin >> number;
            Example.pushD(number);
        }
        if (choice == 2) {
            std::cout << "Last value was: ";
            Example.popD();
        }

        if (choice == 3) {
            Example.checkValuesD();
        }
        if (choice == 4) {
            Example.isEmptyD();
        }
        if (choice == 5) {
            Example.frontD();
        }
        if (choice == 6) {
            int tempIndex;
            std::cout << "Type index" << std::endl;
            std::cin >> tempIndex;
            if (tempIndex > 0 && tempIndex < Example.size())
                std::cout << Example[tempIndex] << std::endl;
            else
                std::cout << "error" << std::endl;
        }
        if (choice == 7) {
            int tempIndex;
            std::cout << "Type index" << std::endl;
            std::cin >> tempIndex;
            if (tempIndex > 0 && tempIndex < Example.size()) {
                std::cout << "Type value" << std::endl;
                std::cin >> Example[tempIndex];
            }
            else
                std::cout << "error" << std::endl;
        }
    }

    return 0;
}
