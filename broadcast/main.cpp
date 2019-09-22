#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <string>
#include <algorithm>

#define seed 12
#define countOfPC 20
#define countOfSwitches 10
#define countOfServers 3

using namespace std;

class RequestResponse {
    public:
        void writeConnection();
        void makeRequest();
        void copyRequest();
        void deleteRequest();
    private:
        vector<string > to;
        vector<string > from;
        string request;
        string nameOfServer;
};
class Server; class Switch; class PC;
class Device {
    public:
        Device() {
            toWhomConnected.reserve(4);
        }
        void generateAddress(int generatedAddress) {
            address = generatedAddress;
        }
        template <typename T>
        void makeNew(vector<T*> &listOfDevices, T* tempDevice, int addressGen) {
            listOfDevices.push_back(tempDevice);
            tempDevice->generateAddress(addressGen);
        }

        int address;
        int countConnect = 0;
        vector<int > toWhomConnected;
};


class Switch : public Device {
    public:
        void countOfResponses();
        void makeConnection(vector<PC*> &listOfPC, vector<Switch*> &listOfSwitches, int i);
        void checkServer();
        map<string, int> counter = {{"facebook", 0}, {"faceapp", 0}, {"tiktok", 0}};
};

class Server : public Device {
    public:
        void makeConnection(vector<Server*> &listOfServers, vector<Switch*> &listOfSwitches, int i) {
            while(listOfServers[i]->countConnect < 2) {
                int randSwitch = rand() % countOfSwitches;
                if (listOfSwitches[randSwitch]->countConnect < 4) {
                    int check = 0;
                    for (int l = 0; l != listOfServers[i]->countConnect; ++l) {
                        if (listOfServers[i]->toWhomConnected[l] == randSwitch)
                            ++check;
                    }
                    if (check == 0) {
                        toWhomConnected.push_back(listOfSwitches[randSwitch]->address);
                        ++countConnect;
                        ++listOfSwitches[randSwitch]->countConnect;
                        listOfSwitches[randSwitch]->toWhomConnected.push_back(listOfServers[i]->address);
                    }
                }
            }
        }
        void generateAddress(int generatedAddress) {
            address = generatedAddress;
            cout << "Server's name: ";
            cin >> name;
        }
        void makeResponse();
        string name;
};


class PC : public Device {
    public:
        void makeConnection(vector<PC*> &listOfPC, vector<Switch*> &listOfSwitches, int i) {
            int randChoice = rand() % 2;
            if(listOfPC[i]->countConnect == 0) {
                if (!randChoice) {
                    int randSwitch = rand() % countOfSwitches;
                    if ((listOfSwitches[randSwitch]->countConnect < 4)) {
                        ++listOfSwitches[randSwitch]->countConnect;
                        ++listOfPC[i]->countConnect;
                        listOfSwitches[randSwitch]->toWhomConnected[listOfSwitches[randSwitch]->countConnect-1] = listOfPC[i]->address;
                        listOfPC[i]->toWhomConnected[0] = randSwitch;
                    }
                    else
                        listOfPC[i]->toWhomConnected[0] = -1;
                }
                if (randChoice) {
                    int randPC = rand() % countOfPC;
                    if (listOfPC[randPC]->countConnect == 0) {
                        vector<int>::iterator it = find(listOfPC[randPC]->toWhomConnected.begin(), listOfPC[randPC]->toWhomConnected.end(), listOfPC[i]->address);
                        if ((listOfPC[randPC]->address != listOfPC[i]->address) && (it != listOfPC[i]->toWhomConnected.end())) {
                            ++listOfPC[i]->countConnect;
                            ++listOfPC[randPC]->countConnect;
                            listOfPC[i]->toWhomConnected[listOfPC[i]->countConnect-1] = listOfPC[randPC]->address;
                            listOfPC[randPC]->toWhomConnected[listOfPC[randPC]->countConnect-1] = listOfPC[i]->address;
                        }
                    }
                }
            }
        }
};

void Switch::makeConnection(vector<PC*> &listOfPC, vector<Switch*> &listOfSwitches, int i) {
    while (listOfSwitches[i]->countConnect < 4) {
        int randSwitch = rand() % countOfSwitches;
        int randPC = rand() % countOfPC;
        int randChoice = rand() % 2;

        if (!randChoice) {
            if (listOfSwitches[randSwitch]->countConnect < 4) {
                int check = 0;
                for (int l = 0; l != listOfSwitches[i]->countConnect; ++l) {
                    if (listOfSwitches[i]->toWhomConnected[l] == randSwitch)
                        ++check;
                }
                if (check == 0) {
                    if (listOfSwitches[randSwitch]->address != listOfSwitches[i]->address) {
                        ++listOfSwitches[randSwitch]->countConnect;
                        ++listOfSwitches[i]->countConnect;
                        listOfSwitches[randSwitch]->toWhomConnected[listOfSwitches[randSwitch]->countConnect-1] = i;
                        listOfSwitches[i]->toWhomConnected[listOfSwitches[i]->countConnect-1] = randSwitch;
                    }
                }
                else
                    break;
            }
        }
        if (randChoice) {
            if (listOfPC[randPC]->countConnect == 0) {
                int check = 0;
                for (int l = 0; l != listOfPC[i]->countConnect; ++l) {
                    if (listOfPC[i]->toWhomConnected[l] == randPC)
                        ++check;
                }
                if (check == 0) {
                    ++listOfSwitches[i]->countConnect;
                    ++listOfPC[randPC]->countConnect;
                    listOfSwitches[i]->toWhomConnected[listOfSwitches[i]->countConnect-1] = listOfPC[randPC]->address;
                    listOfPC[randPC]->toWhomConnected[0] = listOfSwitches[i]->address;
                }
            }
        }
    }
}

class Broadcast {
public:
    template <typename T>
    void makeDevices(vector<T*> &listOfDevices, int &addressGen, int count) {
        for (int i = 0; i != count; ++i) {
            T *tempDevice = new T;
            tempDevice->makeNew(listOfDevices, tempDevice, addressGen);
            ++addressGen;
        }
    }

    void createBroadcast(vector <PC*> &listOfPC, vector <Switch*> &listOfSwitches, vector <Server*> &listOfServers, int &addressGen) {
        makeDevices(listOfSwitches, addressGen, countOfSwitches);
        makeDevices(listOfServers, addressGen, countOfServers);
        makeDevices(listOfPC, addressGen, countOfPC);
        for (int i = 0; i != countOfServers; ++i)
            listOfServers[i]->makeConnection(listOfServers, listOfSwitches, i);
        for (int i = 0; i != countOfPC; ++i)
            listOfPC[i]->makeConnection(listOfPC, listOfSwitches, i);
        for (int i = 0; i != countOfSwitches; ++i)
            listOfSwitches[i]->makeConnection(listOfPC, listOfSwitches, i);
    }

    void showBroadcast(vector<PC*> listOfPC, vector<Switch*> listOfSwitches, vector<Server*> listOfServers) {
        for (auto &a: listOfSwitches) {
            cout << "Switch n" << a->address << " ";
            for (int i = 0; i != a->countConnect; ++i)
                cout << a->toWhomConnected[i] << " ";
            cout << endl;
        }

        for (auto &a: listOfServers)
            cout << "Server " << a->address << " -> " << a->toWhomConnected[0] << " " << a->toWhomConnected[1] << " " << a->name << endl;
        for (auto &a: listOfPC) {
            cout << "PC " << a->address << " -> ";
            if (a->countConnect != 0)
                for (int i = 0; i != a->countConnect; ++i)
                    cout << a->toWhomConnected[i] << " ";
            else
                cout << "no connection";
            cout << endl;
        }
    }

    void clearMemory(vector<PC*> listOfPC, vector<Switch*> listOfSwitches, vector<Server*> listOfServers) {
        for (auto &a: listOfSwitches)
            delete a;
        for (auto &a: listOfServers)
            delete a;
        for (auto &a: listOfPC)
            delete a;
    }
};

int main()
{
    vector<PC*> listOfPC;
    vector<Switch*> listOfSwitches;
    vector<Server*> listOfServers;
    Broadcast broadcast;
    srand(seed);
    int addressGen = 0;

    broadcast.createBroadcast(listOfPC, listOfSwitches, listOfServers, addressGen);
    broadcast.showBroadcast(listOfPC, listOfSwitches, listOfServers);
    broadcast.clearMemory(listOfPC, listOfSwitches, listOfServers);

    return 0;
}
