#include <iostream>
#include <list>
#include <map>
#include <iterator>
#include <limits>
#include <set>

using namespace std;

class MetroGraph
{
    map<string, list<pair<string, float>>> adjacencyList;

public:
    list<string> shortestPath;

    void addEdge(string u, string v, float dist, bool bidir = true)
    {
        adjacencyList[u].push_back(make_pair(v, dist));
        if (bidir)
        {
            adjacencyList[v].push_back(make_pair(u, dist));
        }
    }

    void DijkstraGetShortestPathTo(string destination, map<string, string> &previous)
    {
        for (; destination != ""; destination = previous[destination])
        {
            shortestPath.push_back(destination);
        }
        shortestPath.reverse();
        cout << "\t\t\t";
        copy(shortestPath.begin(), shortestPath.end(), ostream_iterator<string>(cout, "\n\t\t\t"));
    }

    bool printAdj(string source, string destination);
    void dijkstraShortestPath(string source, map<string, float> &distance, map<string, string> &previous);
    bool check(string source, string destination);
};

bool MetroGraph::check(string src, string dest)
{
    int count = 0;
    list<string>::iterator it;
    for (it = shortestPath.begin(); it != shortestPath.end(); ++it)
    {
        if (*it == src || *it == dest)
        {
            count++;
        }
    }
    return count == 2;
}

bool MetroGraph::printAdj(string src, string dest)
{
    int count;
    count = 0;
    for (auto j : adjacencyList)
    {
        if (j.first == src)
            count++;
        for (auto l : j.second)
        {
            if (l.first == dest)
            {
                count++;
            }
        }
    }
    return count > 1;
}

void MetroGraph::dijkstraShortestPath(string src, map<string, float> &dist, map<string, string> &prev)
{
    set<pair<float, string>> s;
    prev.clear();

    for (auto j : adjacencyList)
    {
        dist[j.first] = numeric_limits<float>::infinity();
        prev[j.first] = "";
    }

    dist[src] = 0;
    s.insert(make_pair(0, src));

    while (!s.empty())
    {
        auto p = *(s.begin());
        string node = p.second;
        float nodeDist = p.first;
        s.erase(s.begin());

        for (auto childPair : adjacencyList[node])
        {
            string dest = childPair.first;
            float weight = childPair.second;
            float distance_through_node = nodeDist + childPair.second;

            if (distance_through_node < dist[childPair.first])
            {
                auto f = s.find(make_pair(dist[dest], dest));
                if (f != s.end())
                {
                    s.erase(f);
                }
                dist[dest] = distance_through_node;
                prev[dest] = node;
                s.insert(make_pair(dist[dest], dest));
            }
        }
    }
}

int main()
{   

    cout << "HELLO! I AM METRO BOT" << endl;
    string source, destination;
    MetroGraph Metro;
    //red
    Metro.addEdge("Rithala","Netaji Subhash Place",5.2);
    Metro.addEdge("Netaji Subhash Place","Keshav Puram",1.2);
    Metro.addEdge("Keshav Puram","Kanhaiya Nagar",0.8);
    Metro.addEdge("Kanhaiya Nagar","Inderlok",1.2);
    Metro.addEdge("Inderlok","Shastri Nagar",1.2);
    Metro.addEdge("Shastri Nagar","Pratap Nagar",1.7);
    Metro.addEdge("Pratap Nagar","Pulbangash",0.8);
    Metro.addEdge("Pulbangash","Tis Hazari",0.9);
    Metro.addEdge("Tis Hazari","Kashmere Gate",1.1);
    Metro.addEdge("Kashmere Gate","Shastri Park",2.2);
    Metro.addEdge("Shastri Park","Seelampur",1.6);
    Metro.addEdge("Seelampur","Welcome",1.1);
    //blue
    Metro.addEdge("Rajouri Garden","Ramesh Nagar",1);
    Metro.addEdge("Ramesh Nagar","Moti Nagar",1.2);
    Metro.addEdge("Moti Nagar","Kirti Nagar",1);
    Metro.addEdge("Kirti Nagar","Shadipur",0.7);
    Metro.addEdge("Shadipur","Patel Nagar",1.3);
    Metro.addEdge("Patel Nagar","Rajender Place",0.9);
    Metro.addEdge("Rajender Place","Karol Bagh",1);
    Metro.addEdge("Karol Bagh","Rajiv Chowk",3.4);
    Metro.addEdge("Rajiv Chowk","Barakhamba Road",0.7);
    Metro.addEdge("Barakhamba Road","Mandi House",1);
    Metro.addEdge("Mandi House","Pragati Maiden",0.8);
    Metro.addEdge("Pragati Maiden","Inderprastha",0.8);
    Metro.addEdge("Inderprastha","Yamuna Bank",1.8);
    Metro.addEdge("Yamuna Bank","Laxmi Nagar",1.3);
    Metro.addEdge("Laxmi Nagar","Nirman Vihar",1.1);
    Metro.addEdge("Nirman Vihar","Preet Vihar",1.0);
    Metro.addEdge("Preet Vihar","Karkar Duma",1.2);
    Metro.addEdge("Karkar Duma","Anand Vihar",1.1);
    Metro.addEdge("Anand Vihar","Kaushambi",0.8);
    Metro.addEdge("Kaushambi","Vaishali",1.6);
    Metro.addEdge("Yamuna Bank","Akshardham",1.3);
    Metro.addEdge("Akshardham","Mayur Vihar Phase-1",1.8);
    Metro.addEdge("Mayur Vihar Phase-1","Mayur Vihar Extention",1.2);
    Metro.addEdge("Mayur Vihar Extention","New Ashok Nagar",0.9);
    Metro.addEdge("New Ashok Nagar","Noida Sector-15",1.0);
    Metro.addEdge("Noida Sector-15","Noida Sector-16",1.1);
    Metro.addEdge("Noida Sector-16","Noida Sector-18",1.1);
    Metro.addEdge("Noida Sector-18","Botanical Garden",1.1);
    Metro.addEdge("Botanical Garden","Golf Course",1.2);
    Metro.addEdge("Golf Course","Noida City Center",1.3);
    //green
    Metro.addEdge("Madipur","Shivaji Park",1.1);
    Metro.addEdge("Shivaji Park","Punjabi Bagh",1.6);
    Metro.addEdge("Punjabi Bagh","Ashok Park",0.9);
    Metro.addEdge("Ashok Park","Inderlok",1.4);
    Metro.addEdge("Ashok Park","Sant Guru Ram Singh Marg",1.1);
    Metro.addEdge("Sant Guru Ram Singh Marg","Kirti Nagar",1);
    Metro.addEdge("Kashmere Gate","Lal Qila",1.5);
    Metro.addEdge("Lal Qila","Jama Masjid",0.8);
    Metro.addEdge("Jama Masjid","Delhi Gate",1.4);
    Metro.addEdge("Delhi Gate","ITO",1.3);
    Metro.addEdge("ITO","Mandi House",0.8);
    Metro.addEdge("Mandi House","Janptah",1.4);
    Metro.addEdge("Janptah","Central Secretariat",1.3);
    Metro.addEdge("Central Secretariat","Khan Market",2.1);
    Metro.addEdge("Khan Market","JL Nehru Stadium",1.4);
    Metro.addEdge("JL Nehru Stadium","Jangpura",0.9);
    //yellow
    Metro.addEdge("Vishwavidyalaya","Vidhan Sabha",1);
    Metro.addEdge("Vidhan Sabha","Civil Lines",1.3);
    Metro.addEdge("Civil Lines","Kashmere Gate",1.1);
    Metro.addEdge("Kashmere Gate","Chandni Chowk",1.1);
    Metro.addEdge("Chandni Chowk","Chawri Bazar",1);
    Metro.addEdge("Chawri Bazar","New Delhi",0.8);
    Metro.addEdge("New Delhi","Rajiv Chowk",1.1);
    Metro.addEdge("Rajiv Chowk","Patel Chowk",1.3);
    Metro.addEdge("Patel Chowk","Central Secretariat",0.9);
    Metro.addEdge("Central Secretariat","Udyog Bhawan",0.3);
    Metro.addEdge("Udyog Bhawan","Lok Kalyan Marg",1.6);
    Metro.addEdge("Lok Kalyan Marg","Jor Bagh",1.2);
    Metro.addEdge("Samaypur Badli","Rohini Sector - 18",0.8);
    Metro.addEdge("Rohini Sector - 18","Haiderpur Badli Mor",1.3);
    Metro.addEdge("Haiderpur Badli Mor","Jahangirpuri",1.3);
    Metro.addEdge("Jahangirpuri","Adarsh Nagar",1.3);
    Metro.addEdge("Adarsh Nagar","Azadpur",1.5);
    Metro.addEdge("Azadpur","Model Town",1.4);
    Metro.addEdge("Model Town","GTB Nagar",1.4);
    Metro.addEdge("GTB Nagar","Vishwavidyalaya",0.8);
    Metro.addEdge("Jor Bagh","INA",1.3);
    Metro.addEdge("INA","AIIMS",0.8);
    Metro.addEdge("AIIMS","Green Park",1.0);
    Metro.addEdge("Green Park","Hauz Khas",1.8);
    Metro.addEdge("Hauz Khas","Malviya Nagar",1.7);
    Metro.addEdge("Malviya Nagar","Saket",0.9);
    Metro.addEdge("Saket","Qutab Minar",1.7);
    Metro.addEdge("Qutab Minar","Chhattarpur",1.3);
    Metro.addEdge("Chhattarpur","Sultanpur",1.6);
    Metro.addEdge("Sultanpur","Ghitorni",1.3);
    Metro.addEdge("Ghitorni","Arjan Garh",2.7);
    Metro.addEdge("Arjan Garh","Guru Dronacharya",2.3);
    Metro.addEdge("Guru Dronacharya","Sikandarpur",1.0);
    Metro.addEdge("Sikandarpur","MG Road",1.2);
    Metro.addEdge("MG Road","IFFCO Chowk",1.1);
    Metro.addEdge("IFFCO Chowk","Huda City Centre",1.5);
    map<string, float> distances;
    map<string, string> previous;
    string sourceStation, destinationStation;

    cout << endl;
    cout << "\t\t";
    cout << "Enter source station in capital case: ";
    getline(cin, sourceStation);
    cout << endl;
    cout << "\t\t";
    cout << "Enter destination station in capital case: ";
    getline(cin, destinationStation);

    bool result = Metro.printAdj(sourceStation, destinationStation);

    Metro.dijkstraShortestPath(sourceStation, distances, previous);
    cout << endl
         << "\t\t";
    cout << "Distance from " << sourceStation << " to " << destinationStation << " - " << distances[destinationStation] << " Kms" << endl;
    cout << endl
         << "\t\tPath: " << endl;
    Metro.DijkstraGetShortestPathTo(destinationStation, previous);

    return 0;
}
