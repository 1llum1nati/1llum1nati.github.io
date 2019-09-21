#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <string>
#include <algorithm>

#define seed 10
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

class Device : public RequestResponse {
    public:
        Device() {
            toWhomConnected.reserve(4);
        }
        void generateAddress(int generatedAddress) {
            address = generatedAddress;
        }
        void writeAddress();

        int address;
        vector<int > toWhomConnected;
};
class PC;

class Switch : public Device {
    public:
        void countOfResponses();
        void checkServer();
        void makeNew(vector<Switch*> &listOfSwitches, Switch* tempSwitch, int addressGen) {
            listOfSwitches.push_back(tempSwitch);
            generateAddress(addressGen);
        }
        int countConnect = 0;
        map<string, int> counter = {{"facebook", 0}, {"faceapp", 0}, {"tiktok", 0}};
};

class Server : public Device {
    public:
        void makeConnection(vector<Server*> &listOfServers, vector<Switch*> &listOfSwitches) {
            for (int j = 0; j != 2; ++j) {
                toWhomConnected.push_back(rand() % countOfSwitches);
                ++listOfSwitches[toWhomConnected[j]]->countConnect;
                listOfSwitches[toWhomConnected[j]]->toWhomConnected.push_back(address);
            }
        }
        void generateAddress(int generatedAddress) {
            address = generatedAddress;
            generatedAddress -= countOfSwitches;
            switch(generatedAddress) {
                case 0:
                    name = "facebook";
                    break;
                case 1:
                    name = "faceapp";
                    break;
                case 2:
                    name = "tiktok";
                    break;
            }
        }
        void makeNew(vector<Server*> &listOfServers, Server* tempServer, int addressGen) {
            listOfServers.push_back(tempServer);
            generateAddress(addressGen);
        }
        void makeResponse();
        string name;
};


class PC : public Device {
    public:
        void makeConnection() {

        }
        void makeNew(vector<PC*> &listOfPC, PC* tempPC, int addressGen) {
            listOfPC.push_back(tempPC);
            generateAddress(addressGen);
        }
        int countConnect = 0;
};


int main()
{
    vector<PC*> listOfPC;
    vector<Switch*> listOfSwitches;
    vector<Server*> listOfServers;
    srand(seed);
    int addressGen = 0, randChoice = 0, randSwitch = 0, randPC = 0, temp = 0;

    for (int i = 0; i != countOfSwitches; ++i) {
        Switch *tempSwitch = new Switch;
        tempSwitch->makeNew(listOfSwitches, tempSwitch, addressGen);
        ++addressGen;
    }

    for (int i = 0; i != countOfServers; ++i) {
        Server *tempServer = new Server;
        tempServer->makeNew(listOfServers, tempServer, addressGen);
        ++addressGen;
        tempServer->makeConnection(listOfServers, listOfSwitches);
    }

    for (int i = 0; i != countOfPC; ++i) {
        PC *tempPC = new PC;
        tempPC->makeNew(listOfPC, tempPC, addressGen);
        ++addressGen;
    }

    for (int i = 0; i != countOfPC; ++i) {
        randChoice = rand() % 2;
        if(listOfPC[i]->countConnect == 0) {
            if (!randChoice) {
                randSwitch = rand() % countOfSwitches;
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
                randPC = rand() % countOfPC + (countOfSwitches + countOfServers);
                int temp = randPC - countOfServers - countOfSwitches;
                if (listOfPC[temp]->countConnect == 0) {
                    vector<int>::iterator it = find(listOfPC[temp]->toWhomConnected.begin(), listOfPC[temp]->toWhomConnected.end(), listOfPC[i]->address);
                    if ((randPC != listOfPC[i]->address) && (it != listOfPC[i]->toWhomConnected.end())) {
                        ++listOfPC[i]->countConnect;
                        ++listOfPC[temp]->countConnect;
                        listOfPC[i]->toWhomConnected[listOfPC[i]->countConnect-1] = randPC;
                        listOfPC[temp]->toWhomConnected[listOfPC[temp]->countConnect-1] = listOfPC[i]->address;
                    }
                }
            }
        }
    }

    for (int i = 0; i != countOfSwitches; ++i) {
        while (listOfSwitches[i]->countConnect < 4) {
            randSwitch = rand() % countOfSwitches;
            randPC = rand() % countOfPC + (countOfSwitches + countOfServers);
            randChoice = rand() % 2;

            if (!randChoice) {
                //vector<int>::iterator it1 = find(listOfSwitches[randSwitch]->toWhomConnected.begin(), listOfSwitches[randSwitch]->toWhomConnected.end(), listOfSwitches[i]->address);
                //vector<int>::iterator it2 = find(listOfSwitches[i]->toWhomConnected.begin(), listOfSwitches[i]->toWhomConnected.end(), listOfSwitches[randSwitch]->address);
                if (listOfSwitches[randSwitch]->countConnect < 4) {
                    int check = 0;
                    for (int l = 0; l != listOfSwitches[i]->countConnect; ++l) {
                        if (listOfSwitches[i]->toWhomConnected[l] == randSwitch)
                            ++check;
                    }
                    for (int l = 0; l != listOfSwitches[randSwitch]->countConnect; ++l) {
                        if (listOfSwitches[randSwitch]->toWhomConnected[l] == i)
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
                temp = randPC-countOfSwitches-countOfServers;
                if ((find(listOfPC[temp]->toWhomConnected.begin(), listOfPC[temp]->toWhomConnected.end(), listOfSwitches[i]->address) == listOfPC[temp]->toWhomConnected.end()) && (listOfPC[temp]->countConnect == 0)) {
                    ++listOfSwitches[i]->countConnect;
                    ++listOfPC[temp]->countConnect;
                    listOfSwitches[i]->toWhomConnected[listOfSwitches[i]->countConnect-1] = randPC;
                    listOfPC[temp]->toWhomConnected[0] = listOfSwitches[i]->address;
                }
            }
        }
    }

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

    for (auto &a: listOfSwitches)
        delete a;
    for (auto &a: listOfServers)
        delete a;
    for (auto &a: listOfPC)
        delete a;

    return 0;
}
