#include <bits/stdc++.h>
using namespace std;


class Metro {
public:
    class Station {
    public:
        mutable unordered_map<string, int> connections;
    };

    unordered_map<string, Station> stations;

    // Constructor to initialize the Metro class.
    Metro() {
        stations.clear();
    }

    // Returns the number of stations in the Metro.
    int numStations() {
        return stations.size();
    }

    // Checks if a station with the given name exists in the Metro.
    bool containsStation(const string& stationName) {
        return stations.find(stationName) != stations.end();
    }

    // Adds a new station with the given name to the Metro.
    void addStation(const string& stationName) {
        Station station;
        stations[stationName] = station;
    }

    // Removes a station with the given name from the Metro.
    void removeStation(const string& stationName) {
        auto stationIter = stations.find(stationName);
        if (stationIter != stations.end()) {
            Station station = stationIter->second;
            vector<string> neighborNames;
            for (const auto& entry : station.connections) {
                neighborNames.push_back(entry.first);
            }

            for (const string& neighborName : neighborNames) {
                Station& connectionstation = stations[neighborName];
                connectionstation.connections.erase(stationName);
            }

            stations.erase(stationIter);
        }
    }

    // Returns the number of edges (connections) between stations in the Metro.
    int numConnections() {
        int count = 0;

        for (const auto& entry : stations) {
            const Station& station = entry.second;
            count += station.connections.size();
        }

        return count / 2;  // Divided by 2 because each edge is counted twice.
    }

    // Checks if there is a direct connection (edge) between two stations.
    bool hasConnection(const string& stationName1, const string& stationName2) {
        auto station1Iter = stations.find(stationName1);
        auto station2Iter = stations.find(stationName2);

        if (station1Iter == stations.end() || station2Iter == stations.end() ||
            station1Iter->second.connections.find(stationName2) == station1Iter->second.connections.end()) {
            return false;
        }

        return true;
    }

    // Adds a connection (edge) between two stations with a given value (weight).
    void addConnection(const string& stationName1, const string& stationName2, int value) {
        auto station1Iter = stations.find(stationName1);
        auto station2Iter = stations.find(stationName2);

        if (station1Iter == stations.end() || station2Iter == stations.end() ||
            station1Iter->second.connections.find(stationName2) != station1Iter->second.connections.end()) {
            return;
        }

        station1Iter->second.connections[stationName2] = value;
        station2Iter->second.connections[stationName1] = value;
    }

    // Removes a connection (edge) between two stations.
    void removeConnection(const string& stationName1, const string& stationName2) {
        auto station1Iter = stations.find(stationName1);
        auto station2Iter = stations.find(stationName2);

        if (station1Iter != stations.end() && station2Iter != stations.end() &&
            station1Iter->second.connections.find(stationName2) != station1Iter->second.connections.end()) {
            station1Iter->second.connections.erase(stationName2);
            station2Iter->second.connections.erase(stationName1);
        }
    }

    // Displays the Metro map with stations and their connections.
    void displayMap() {
        cout << "\t Metro Map" << endl;
        cout << "\t------------------" << endl;
        cout << "----------------------------------------------------\n" << endl;

        for (const auto& entry : stations) {
            const string& stationName = entry.first;
            const Station& station = entry.second;
            cout << stationName << " =>" << endl;
            vector<string> neighborNames;
            for (const auto& neighborEntry : station.connections) {
                neighborNames.push_back(neighborEntry.first);
            }

            for (const string& neighborName : neighborNames) {
                string str = "\t" + neighborName + "\t";
                if (neighborName.length() < 16)
                    str += "\t";
                if (neighborName.length() < 8)
                    str += "\t";
                str += to_string(station.connections.at(neighborName)) + "\n";
                cout << str;
            }
            cout << endl;
        }

        cout << "\t------------------" << endl;
        cout << "---------------------------------------------------\n" << endl;
    }

    // Displays the list of stations in the Metro.
    void displayStations() {
        cout << "\n***********************************************************************\n" << endl;
        int i = 1;
        for (const auto& entry : stations) {
            const string& stationName = entry.first;
            cout << i << ". " << stationName << endl;
            i++;
        }
        cout << "\n***********************************************************************\n" << endl;
    }

    // Checks if there is a path between two stations using depth-first search.
    bool hasPath(const string& stationName1, const string& stationName2, unordered_map<string, bool>& processed) {
        if (hasConnection(stationName1, stationName2)) {
            return true;
        }

        processed[stationName1] = true;

        const Station& station = stations[stationName1];
        vector<string> connections;
        for (const auto& neighborEntry : station.connections) {
            connections.push_back(neighborEntry.first);
        }

        for (const string& neighbor : connections) {
            if (!processed.count(neighbor) && hasPath(neighbor, stationName2, processed)) {
                return true;
            }
        }

        return false;
    }

    class DijkstraPair {
    public:
        string stationName;
        string pathSoFar;
        int cost;

        bool operator>(const DijkstraPair& other) const {
            return cost > other.cost;
        }
    };

    int findShortestPath(const string& src, const string& des, bool useAdditionalNodes) {
        int minCost = 0;
        vector<string> result;
        unordered_map<string, DijkstraPair> map;

        priority_queue<DijkstraPair, vector<DijkstraPair>, greater<DijkstraPair>> pq;

        // Initialize the Dijkstra pairs for all stations
        for (const auto& entry : stations) {
            const string& stationName = entry.first;
            DijkstraPair pair;
            pair.stationName = stationName;
            pair.cost = numeric_limits<int>::max();

            if (stationName == src) {
                pair.cost = 0;
                pair.pathSoFar = stationName;
            }

            pq.push(pair);
            map[stationName] = pair;
        }

        // Dijkstra's algorithm to find the shortest path
        while (!pq.empty()) {
            DijkstraPair currentPair = pq.top();
            pq.pop();

            if (currentPair.stationName == des) {
                minCost = currentPair.cost;
                break;
            }

            map.erase(currentPair.stationName);
            result.push_back(currentPair.stationName);

            const Station& currentStation = stations[currentPair.stationName];
            for (const auto& neighborEntry : currentStation.connections) {
                const string& neighborName = neighborEntry.first;
                if (map.count(neighborName)) {
                    int oldCost = map[neighborName].cost;
                    const Station& connectionstation = stations[currentPair.stationName];
                    int newCost;

                    if (useAdditionalNodes)
                        newCost = currentPair.cost + 120 + 40 * connectionstation.connections[neighborName];
                    else
                        newCost = currentPair.cost + connectionstation.connections[neighborName];

                    if (newCost < oldCost) {
                        DijkstraPair& updatedPair = map[neighborName];
                        updatedPair.pathSoFar = currentPair.pathSoFar + neighborName;
                        updatedPair.cost = newCost;

                        pq.push(updatedPair);
                    }
                }
            }
        }
        return minCost;
    }

    class Pair {
    public:
        string stationName;
        string pathSoFar;
        int minDistance;
        int minTime;
    };

    string findShortestPathWithMinDistance(string& src, string& dst) {
        int minDistance = numeric_limits<int>::max();
        string shortestPath = "";
        unordered_map<string, bool> processed;
        deque<Pair> stack;

        Pair startingPair;
        startingPair.stationName = src;
        startingPair.pathSoFar = src + "  ";
        startingPair.minDistance = 0;
        startingPair.minTime = 0;

        stack.push_front(startingPair);

        while (!stack.empty()) {
            Pair currentPair = stack.front();
            stack.pop_front();

            if (processed.find(currentPair.stationName) != processed.end()) {
                continue;
            }

            processed[currentPair.stationName] = true;

            if (currentPair.stationName == dst) {
                int tempDistance = currentPair.minDistance;
                if (tempDistance < minDistance) {
                    shortestPath = currentPair.pathSoFar;
                    minDistance = tempDistance;
                }
                continue;
            }

            Station& currentStation = stations[currentPair.stationName];
            vector<string> connections;
            for (const auto& neighborEntry : currentStation.connections) {
                connections.push_back(neighborEntry.first);
            }

            for (const string& neighbor : connections) {
                if (processed.find(neighbor) == processed.end()) {
                    Pair newPair;
                    newPair.stationName = neighbor;
                    newPair.pathSoFar = currentPair.pathSoFar + neighbor + "  ";
                    newPair.minDistance = currentPair.minDistance + currentStation.connections[neighbor];
                    stack.push_front(newPair);
                }
            }
        }

        shortestPath = shortestPath + to_string(minDistance);
        return shortestPath;
    }

    string findShortestPathWithMinTime(const string& src, const string& dst) {
        int minTime = numeric_limits<int>::max();
        string shortestPath = "";
        unordered_map<string, bool> processed;
        deque<Pair> stack;

        Pair startingPair;
        startingPair.stationName = src;
        startingPair.pathSoFar = src + "  ";
        startingPair.minDistance = 0;
        startingPair.minTime = 0;

        stack.push_front(startingPair);

        while (!stack.empty()) {
            Pair currentPair = stack.front();
            stack.pop_front();

            if (processed.find(currentPair.stationName) != processed.end()) {
                continue;
            }

            processed[currentPair.stationName] = true;

            if (currentPair.stationName == dst) {
                int tempTime = currentPair.minTime;
                if (tempTime < minTime) {
                    shortestPath = currentPair.pathSoFar;
                    minTime = tempTime;
                }
                continue;
            }

            const Station& currentStation = stations[currentPair.stationName];
            vector<string> connections;
            for (const auto& neighborEntry : currentStation.connections) {
                connections.push_back(neighborEntry.first);
            }

            for (const string& neighbor : connections) {
                if (processed.find(neighbor) == processed.end()) {
                    Pair newPair;
                    newPair.stationName = neighbor;
                    newPair.pathSoFar = currentPair.pathSoFar + neighbor + "  ";
                    newPair.minDistance = currentPair.minDistance + currentStation.connections[neighbor];
                    newPair.minTime = currentPair.minTime + 120 + 40 * currentStation.connections[neighbor];
                    stack.push_front(newPair);
                }
            }
        }

        double minutes = ceil(static_cast<double>(minTime) / 60);
        shortestPath = shortestPath + to_string(minutes);
        return shortestPath;
    }

    vector<string> getInterchanges(const string& inputString) {
        vector<string> tokens;
        vector<string> result;
        istringstream iss(inputString);
        string token;

        while (getline(iss, token, ' ')) {
            tokens.push_back(token);
        }

        result.push_back(tokens[0]);
        int interchangeCount = 0;

        for (size_t i = 1; i < tokens.size() - 1; i++) {
            size_t index = tokens[i].find('~');
            string substring = tokens[i].substr(index + 1);

            if (substring.length() == 2) {
                string previous = tokens[i - 1].substr(tokens[i - 1].find('~') + 1);
                string next = tokens[i + 1].substr(tokens[i + 1].find('~') + 1);

                if (previous == next) {
                    result.push_back(tokens[i]);
                } else {
                    result.push_back(tokens[i] + " ==> " + tokens[i + 1]);
                    i++;
                    interchangeCount++;
                }
            } else {
                result.push_back(tokens[i]);
            }
        }

        result.push_back(to_string(interchangeCount));
        result.push_back(tokens[tokens.size() - 1]);

        return result;
    }
    void Create_Metro_Map(Metro& g) {
        g.addStation("Nangloi");
        g.addStation("Paschim Vihar West");
        g.addStation("Punjab Bagh West");
        g.addStation("Inderlok");
        g.addStation("Kirti Nagar");
        g.addStation("Dwarka");
        g.addStation("Janak Puri West");
        g.addStation("Rajouri Garden");
        g.addStation("Rajiv Chowk");
        g.addStation("Mandi House");
        g.addStation("Yamuna Bank");
        g.addStation("Noida");
        g.addStation("Dilshad Garden");
        g.addStation("WELCOME");
        g.addStation("Kashmere Gate");
        g.addStation("Inderlok");
        g.addStation("Netaji Subhash Place");
        g.addStation("Rohini");
        g.addStation("Lajpat Nagar");
        g.addStation("Central Secretariat");
        g.addStation("Kalkaji Mandir");
        g.addStation("DDS Campus");
        g.addStation("Delhi Haat");
        g.addStation("Mayur Vihar");
        g.addStation("Anand Vihar ISBT");
        g.addStation("Hauz Khas");
        g.addStation("Botanical Garden");
        g.addStation("IGI Airport");
        g.addStation("New Delhi");
        

        g.addConnection("Nangloi", "Paschim Vihar West", 4);
        g.addConnection("Paschim Vihar West", "Punjabi Bagh West", 4);
        g.addConnection("Punjabi Bagh West", "Inderlok", 3);
        g.addConnection("Inderlok", "Kashmere Gate", 5);
        g.addConnection("Punjabi Bagh West", "Kirti Nagar", 5);
        g.addConnection("Inderlok", "Netaji Subhash Place", 4);
        g.addConnection("Netaji Subhash Place", "Rohini", 4);
        g.addConnection("Kashmere Gate", "WELCOME", 4);
        g.addConnection("WELCOME", "Dilshad Garden", 4);
        g.addConnection("WELCOME", "Mayur Vihar", 11);
        g.addConnection("Mayur Vihar", "Lajpat Nagar", 4);
        g.addConnection("Lajpat Nagar", "Delhi Haat", 2);
        g.addConnection("Delhi Haat", "Rajouri Garden", 8);
        g.addConnection("Rajouri Garden", "Punjabi Bagh West", 2);
        g.addConnection("Punjabi Bagh West", "Netaji Subhash Place", 2);
        g.addConnection("Dwarka", "Janakpuri West", 5);
        g.addConnection("Janakpuri West", "Rajouri Garden", 5);
        g.addConnection("Rajouri Garden", "Kirti Nagar", 3);
        g.addConnection("Kirti Nagar", "Rajiv Chowk", 7);
        g.addConnection("Rajiv Chowk", "Mandi House", 2);
        g.addConnection("Mandi House", "Yamuna Bank", 3);
        g.addConnection("Yamuna Bank", "Anand Vihar ISBT", 5);
        g.addConnection("Hauz Khas", "Delhi Haat", 3);
        g.addConnection("Delhi Haat", "Central Secretariat", 4);
        g.addConnection("Central Secretariat", "Rajiv Chowk", 2);
        g.addConnection("Rajiv Chowk", "New Delhi", 1);
        g.addConnection("New Delhi", "Kashmere Gate", 3);
        g.addConnection("Kashmere Gate", "Rohini", 12);
        g.addConnection("Kalkaji Mandir", "Lajpat Nagar", 4);
        g.addConnection("Lajpat Nagar", "Central Secretariat", 4);
        g.addConnection("Central Secretariat", "Mandi House", 2);
        g.addConnection("Mandi House", "Kashmere Gate", 5);
        g.addConnection("New Delhi", "IGI Airport", 4);
        
    }
};

int main() {
    Metro g; // Instance of Metro
    g.Create_Metro_Map(g); // Calling the member function on the instance

    cout << "\n\t\t\t****WELCOME TO THE METRO APP*****" << endl;
    while (true) {
        cout << "\t\t\t\t~~LIST OF ACTIONS~~\n\n";
        cout << "1. LIST ALL THE STATIONS IN THE MAP" << endl;
        cout << "2. SHOW THE METRO MAP" << endl;
        cout << "3. GET SHORTEST DISTANCE AND PATH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "4. GET SHORTEST TIME AND PATH TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "5. EXIT THE MENU" << endl;
        cout << "\nENTER YOUR CHOICE FROM THE ABOVE LIST (1 to 5) : ";
        int choice = -1;
        cin >> choice;
        cout << "\n***********************************************************\n";

        switch (choice) {
            case 1:
                g.displayStations();  
                break;

            case 2:
                g.displayMap();  
                break;

            case 3:{
                string s1, s2;
                cout << "Enter source and destination station" << endl;
                cin.ignore();  
                getline(cin, s1);
                getline(cin, s2);

                unordered_map<string, bool> processed2;
                if (!g.containsStation(s1) || !g.containsStation(s2) || !g.hasPath(s1, s2, processed2))
                    cout << "Red Flag ---> INVALID INPUT" << endl;
                else {
                    vector<string> str = g.getInterchanges(g.findShortestPathWithMinDistance(s1, s2));
                    int len = str.size();
                    cout << "Start station : " << s1 << endl;
                    cout << "Destination station : " << s2 << endl;
                    cout << "You will have " << str[len - 1] << " stations is between your source and destination" << endl;
                    cout << "~~~~~~~~~~~~~" << endl;
                    cout << "Your Starting station is :  \n" << str[0] << endl;
                    for (int i = 1; i < len - 3; i++) {
                        cout << str[i] << endl;
                    }
                    cout << str[len - 3] << " is your ending station" << endl;
                    cout << "~~~~~~~~~~~~~" << endl;
                }
                break;
            }
            
            case 4:{
                string ss1, ss2;
                cout << "Enter the starting station ";
                cin.ignore();  
                getline(cin, ss1);
                cout << "Enter the destination station ";
                getline(cin, ss2);

                unordered_map<string, bool> processed3;
                if (!g.containsStation(ss1) || !g.containsStation(ss2) || !g.hasPath(ss1, ss2, processed3))
                    cout << "Red Flag ---> INVALID INPUT" << endl;
                else {
                    vector<string> str = g.getInterchanges(g.findShortestPathWithMinTime(ss1, ss2));
                    int len = str.size();
                    cout << "Start station : " << ss1 << endl;
                    cout << "Destination station : " << ss2 << endl;
                    cout << "The journey would take you " << str[len - 1] << " minutes" << endl;
                    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
                    cout << "Your Starting station is  " << str[0] << "\n";
                    for (int i = 1; i < len - 3; i++) {
                        cout << str[i] << endl;
                    }
                    cout << str[len - 3] << "\nis your ending station" << endl;
                    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
                }
                break;
            }

            case 5:
                exit(0);

            default:
                cout << "Please enter a valid option!" << endl;
                cout << "The options you can choose are from 1 to 5." << endl;
        }
    }

    return 0;
}


