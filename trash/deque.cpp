#include <iostream>
#include <ctime>
#include <cassert>

class Node {
public:
    int value;
    Node *next;
    Node *prev;
};

class Deque {
public:
    Node *head = NULL;
    Node *tail = NULL;
    int sizeOfDeque = 0;

    void push_front(int value) {
        if (head == NULL) {
            Node *Temp = new Node;
            Temp->value = value;
            Temp->next = NULL;
            Temp->prev = NULL;
            head = Temp;
            tail = Temp;
        }
        else {
            Node *Temp = new Node;
            head->prev = Temp;
            Temp->value = value;
            Temp->next = head;
            Temp->prev = NULL;
            head = Temp;
        }
        ++sizeOfDeque;
    }

    void push_back(int value) {
        if (head == NULL) {
            Node *Temp = new Node;
            Temp->value = value;
            Temp->next = NULL;
            Temp->prev = NULL;
            head = Temp;
            tail = Temp;
        }
        else {
            Node *Temp = new Node;
            tail->next = Temp;
            Temp->value = value;
            Temp->prev = tail;
            Temp->next = NULL;
            tail = Temp;
        }
        ++sizeOfDeque;
    }

    int pop_front() {
        if (sizeOfDeque > 0) {
            int tempInt = head->value;
            Node *Temp = new Node;
            Temp = head;
            head = head->next;
            delete Temp;
            std::cout << tempInt << std::endl;
            --sizeOfDeque;
            return tempInt;
        }
        else
            std::cout << "Deque is empty!" << std::endl;
    }
    int pop_back() {
        if (sizeOfDeque > 0) {
            int tempInt = tail->value;
            Node *Temp = new Node;
            Temp = tail;
            tail = tail->prev;
            delete Temp;
            std::cout << tempInt << std::endl;
            --sizeOfDeque;
            return tempInt;
        }
        else
            std::cout << "Deque is empty!" << std::endl;
    }

    void init(int initSize) {
        int temp = 0;
        for (int i = 0; i != initSize; ++i) {
             temp = rand() % 100;
             push_back(temp);
        }
    }

    void isEmpty() {
        if (!sizeOfDeque)
            std::cout << "Deque is empty!" << std::endl;
        else
            std::cout << "Deque is not empty!" << std::endl;
    }

    void checkValue() {
        if (!sizeOfDeque)
            std::cout << "Deque is empty!" << std::endl;
        else {
            Node *Temp = head;
            for(int i = 0; i != sizeOfDeque; ++i) {
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
    void back() {
        std::cout << tail->value << std::endl;
    }
    int &operator[] (int index);
    int tempInt = -1;
};

int &Deque::operator[] (int index) {
    assert(index < sizeOfDeque);
    if (index < sizeOfDeque) {
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
    Deque Example;
    int choice = 0, choiceInit = 0;
    std::cout << "Type any number to init the deque, 0 to continue\n";
    std::cin >> choiceInit;
    if (choiceInit) {
        std::cout << "Size?\n";
        int sizeOfDeque;
        std::cin >> sizeOfDeque;
        Example.init(sizeOfDeque);
        std::cout << "Stack has been initialized!\n";
    }
    while(choice != 10) {
        std::cout << "Enter number:\n" <<
            "1 - push_front\n" <<
            "2 - push_back\n" <<
            "3 - pop_front\n" <<
            "4 - pop_back\n" <<
            "5 - check all\n" <<
            "6 - check front\n" <<
            "7 - check back\n" <<
            "8 - get value\n" <<
            "9 - set value\n" <<
            "10 - stop\n" <<
            "> ";
        std::cin >> choice;
        if (choice == 1) {
            std::cout << "Type any integer number: ";
            int number;
            std::cin >> number;
            Example.push_front(number);
        }
        if (choice == 2) {
            std::cout << "Type any integer number: ";
            int number;
            std::cin >> number;
            Example.push_back(number);
        }
        if (choice == 3) {
            std::cout << "Last front value was: ";
            Example.pop_front();
        }
        if (choice == 4) {
            std::cout << "Last back value was: ";
            Example.pop_back();
        }
        if (choice == 5) {
            Example.checkValue();
        }
        if (choice == 6) {
            Example.front();
        }
        if (choice == 7) {
            Example.back();
        }
        if (choice == 8) {
            int tempIndex;
            std::cout << "Type index" << std::endl;
            std::cin >> tempIndex;
            std::cout << Example[tempIndex] << std::endl;
        }
        if (choice == 9) {
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
