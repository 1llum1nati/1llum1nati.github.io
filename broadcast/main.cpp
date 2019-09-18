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
            toWhomConnected.reserve(2);
        }
        void generateAddress(int generatedAddress) {
            address = generatedAddress;
        }
        void makeConnection();
        void writeAddress();

        int address;
        vector<int > toWhomConnected;
};

class Server : public Device {
    public:
        void makeConnection() {

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
        void makeResponse();
        string name;
};

class Switch : public Device {
    public:
        void makeConnection();
        void countOfResponses();
        void checkServer();

        int countConnect = 0;
        int countConnectOut = 0;
        map<string, int> counter = {{"facebook", 0}, {"faceapp", 0}, {"tiktok", 0}};
};

class PC : public Device {
    public:
        void makeConnection() {

        }
};

vector<PC*> listOfPC;
vector<Switch*> listOfSwitches;
vector<Server*> listOfServers;

int main()
{
    srand(seed);
    int addressGen = 0;

    for (int i = 0; i != countOfSwitches; ++i) {
        Switch *tempSwitch = new Switch;
        listOfSwitches.push_back(tempSwitch);
        listOfSwitches[i]->generateAddress(addressGen);
        ++addressGen;
    }
    for (int i = 0; i != countOfServers; ++i) {
        Server *tempServer = new Server;
        listOfServers.push_back(tempServer);

        listOfServers[i]->generateAddress(addressGen);
        ++addressGen;

        listOfServers[i]->toWhomConnected.insert(listOfServers[i]->toWhomConnected.end(), rand() % countOfSwitches);
        listOfServers[i]->toWhomConnected.insert(listOfServers[i]->toWhomConnected.end(), rand() % countOfSwitches);
        ++listOfSwitches[listOfServers[i]->toWhomConnected[0]]->countConnect;
        ++listOfSwitches[listOfServers[i]->toWhomConnected[1]]->countConnect;
        //cout << listOfServers[i]->toWhomConnected[0] << " " << listOfServers[i]->toWhomConnected[1] << endl;
        //cout << listOfSwitches[listOfServers[i]->toWhomConnected[0]]->countConnect << " " << listOfSwitches[listOfServers[i]->toWhomConnected[1]]->countConnect << endl;
        //cout << "-----------------" << endl;
    }
    for (int i = 0; i != countOfPC; ++i) {
        PC *tempPC = new PC;
        listOfPC.push_back(tempPC);

        listOfPC[i]->generateAddress(addressGen);
        ++addressGen;
    }

    for (int i = 0; i != countOfPC; ++i) {
        int randChoice = rand() % 2;
        if (!randChoice) {
            int randSwitch = rand() % countOfSwitches;
            if (listOfSwitches[randSwitch]->countConnect < 4) {
                ++listOfSwitches[randSwitch]->countConnect;
                listOfPC[i]->toWhomConnected[0] = randSwitch;
            }
            else
                listOfPC[i]->toWhomConnected[0] = -1;
        }
        if (randChoice) {
            int randPC = rand() % countOfPC + (countOfSwitches + countOfServers);
            int temp = randPC - countOfServers - countOfSwitches;
            if ((randPC != listOfPC[i]->address) && (listOfPC[temp]->toWhomConnected[0] != listOfPC[i]->address)) {
                listOfPC[i]->toWhomConnected[0] = randPC;
            }
            else
                listOfPC[i]->toWhomConnected[0] = -1;
        }
    }
    for (int i = 0; i != countOfSwitches; ++i) {
        if (listOfSwitches[i]->countConnect < 4) {
            int randSwitch = rand() % countOfSwitches;
            int randPC = rand() % countOfPC + (countOfSwitches + countOfServers);
            int randChoice = rand() % 2;
            if (!randChoice) {
                if (listOfSwitches[randSwitch]->countConnect < 4) {
                    ++listOfSwitches[randSwitch]->countConnect;
                    ++listOfSwitches[i]->countConnect;
                    ++listOfSwitches[i]->countConnectOut;
                    listOfSwitches[i]->toWhomConnected[listOfSwitches[i]->countConnectOut-1] = randSwitch;
                }
            }
            if (randChoice) {
                int temp = randPC-countOfSwitches-countOfServers;
                if (find(listOfPC[temp]->toWhomConnected.begin(), listOfPC[temp]->toWhomConnected.end(), listOfSwitches[i]->address) == listOfPC[temp]->toWhomConnected.end()) {
                    ++listOfSwitches[i]->countConnectOut;
                    ++listOfSwitches[i]->countConnect;
                    listOfSwitches[i]->toWhomConnected[listOfSwitches[i]->countConnectOut-1] = randPC;
                }
            }
        }
    }

    for (auto &a: listOfSwitches) {
        cout << "Switch n" << a->address << " has " << a->countConnect << " connections in summary and " << a->countConnectOut << " external connections" << endl;
    }
    for (auto &a: listOfServers)
        cout << "Server " << a->address << "->" << a->toWhomConnected[0] << " " << a->toWhomConnected[1] << " " << a->name << endl;
    for (auto &a: listOfPC) {
        if (a->toWhomConnected[0] == -1)
            cout << "PC " << a->address << "->" << a->toWhomConnected[0] << " (no connection)" << endl;
        else
            cout << "PC " << a->address << "->" << a->toWhomConnected[0] << endl;
    }
    return 0;
}
