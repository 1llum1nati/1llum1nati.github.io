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

class Server; class Switch; class PC; class Broadcast;
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
        void checkServer();
        map<string, int> counter = {{"facebook", 0}, {"faceapp", 0}, {"tiktok", 0}};
};

class Server : public Device {
    public:

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

};

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
            makeConnectionServer(listOfServers, listOfSwitches, i);
        for (int i = 0; i != countOfPC; ++i)
            makeConnectionPC(listOfPC, listOfSwitches, i);
        for (int i = 0; i != countOfSwitches; ++i)
            makeConnectionSwitch(listOfPC, listOfSwitches, i);
    }

    void showBroadcast(vector<PC*> listOfPC, vector<Switch*> listOfSwitches, vector<Server*> listOfServers) {
        for (auto &a: listOfSwitches) {
            cout << "Switch " << a->address << " ";
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

    void makeConnectionSwitch(vector<PC*> &listOfPC, vector<Switch*> &listOfSwitches, int i) {
        while (listOfSwitches[i]->countConnect < 4) {
            int randSwitch = rand() % countOfSwitches;
            int randPC = rand() % countOfPC;
            int randChoice = rand() % 2;

            if (!randChoice) {
                if (listOfSwitches[randSwitch]->countConnect < 4) {
                        int checkCounter = check(listOfSwitches, randSwitch, i);
                        if (!checkCounter) {
                            if (listOfSwitches[randSwitch]->address != listOfSwitches[i]->address) {
                                rememberConnection(listOfSwitches, listOfSwitches, randSwitch, i);
                        }
                    }
                    else
                        break;
                }
            }
            if (randChoice) {
                if (listOfPC[randPC]->countConnect == 0) {
                    int checkCounter = check(listOfPC, randPC, i);
                    if (!checkCounter) {
                        rememberConnection(listOfSwitches, listOfPC, randPC, i);
                    }
                }
            }
        }
    }

    void makeConnectionPC(vector<PC*> &listOfPC, vector<Switch*> &listOfSwitches, int i) {
        int randChoice = rand() % 2;
        if(listOfPC[i]->countConnect == 0) {
            if (!randChoice) {
                int randSwitch = rand() % countOfSwitches;
                if ((listOfSwitches[randSwitch]->countConnect < 4)) {
                    int checkCounter = check(listOfPC, randSwitch, i);
                    if (!checkCounter) {
                        rememberConnection(listOfPC, listOfSwitches, randSwitch, i);
                    }
                }
                else
                    listOfPC[i]->toWhomConnected[0] = -1;
            }
            if (randChoice) {
                int randPC = rand() % countOfPC;
                if (listOfPC[randPC]->address != listOfPC[i]->address) {
                    if (listOfPC[randPC]->countConnect == 0) {
                        int checkCounter = check(listOfPC, randPC, i);
                        if (!checkCounter) {
                            rememberConnection(listOfPC, listOfPC, randPC, i);
                        }
                    }
                }
            }
        }
    }

    void makeConnectionServer(vector<Server*> &listOfServers, vector<Switch*> &listOfSwitches, int i) {
        while(listOfServers[i]->countConnect < 2) {
            int randSwitch = rand() % countOfSwitches;
            if (listOfSwitches[randSwitch]->countConnect < 4) {
                int checkCounter = check(listOfServers, randSwitch, i);
                if (!checkCounter) {
                    rememberConnection(listOfServers, listOfSwitches, randSwitch, i);
                }
            }
        }
    }

    template <typename T1, typename T2>
    void rememberConnection(vector<T1*> listOfDevices1, vector<T2*> listOfDevices2, int randDevice, int i) {
        ++listOfDevices1[i]->countConnect;
        ++listOfDevices2[randDevice]->countConnect;
        listOfDevices1[i]->toWhomConnected[listOfDevices1[i]->countConnect-1] = listOfDevices2[randDevice]->address;
        listOfDevices2[randDevice]->toWhomConnected[listOfDevices2[randDevice]->countConnect-1] = listOfDevices1[i]->address;
    }

    template <typename T>
    int check(vector<T*> listOfDevices, int randDevice, int i) {
        int checkCounter = 0;
        for (int l = 0; l != listOfDevices[i]->countConnect; ++l) {
            if (listOfDevices[i]->toWhomConnected[l] == randDevice)
                ++checkCounter;
        }
        return checkCounter;
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
