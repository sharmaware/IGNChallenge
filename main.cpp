//I/O
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <vector>
#include <list>
#include <unordered_map>
#include <functional>

#include <chrono>

using namespace std;
using namespace std::chrono;

class Quest{
public:
    string name;
    int start;
    int duration;
    int reward;
    string difficulty;
    string location;
    string giver;
    
    bool operator< (const Quest &other) const {
        if(start < other.start){
            return true;
        }
        return false;
    }
    
    bool operator== (const Quest &other) const {
        if(name == other.name){
            return true;
        }
        return false;
    }
    
};

struct Hasher{
    size_t operator()(const Quest &q) const{
        return hash<string>()(q.name); //hashing based on name of quest
    }
};

struct OutputObj{
    vector<Quest> quests;
    int reward;
};

void loadQuests(string path, list<Quest> &quests){
    ifstream infile (path);
    
    while(true){
        Quest q;
        getline(infile, q.name);
        
        if(q.name == ""){
            break;
        }
        infile >> q.start >> q.duration >> q.reward;
        infile.ignore(10000, '\n');
        getline(infile, q.difficulty);
        getline(infile, q.location);
        getline(infile, q.giver);
        quests.push_back(q);
    }
    
}

int maxReward(list<Quest> quests, vector<Quest> &order, unordered_map<Quest, OutputObj, Hasher> &map);

int main(int argc, const char * argv[]) {
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    
    //using list for fast erasing from front
    list<Quest> quests;
    loadQuests("/Users/saarthaksharma/Desktop/IGNCodeChallenge/IGNCodeChallenge/quests.txt", quests);
    quests.sort();
           
    ostringstream header;
    header << left << setw(30) << "Name" <<  setw(10) << "Start Day" << setw(10) << "Duration" <<  setw(10) << "Reward" <<  endl << endl;
    cout << left << header.str();
    for(auto it = quests.begin(); it != quests.end(); it++){
        cout << setw(30) << it->name << setw(10) << it->start << setw(10) << it->duration << setw(10) << it->reward << setw(10) << endl;
    }
    
    cout << endl << "Solution" << endl<< endl;
    unordered_map<Quest, OutputObj, Hasher> map;
    
    vector<Quest> order;
    int reward = maxReward(quests, order, map);
    
    cout << setw(30) << "Name" <<  setw(10) << "Start Day" << setw(10) << "Duration" <<  setw(10) << "Reward" <<  endl << endl;
    for(int i = 0; i < order.size(); i++){
        cout << setw(30) << order[i].name <<  setw(10) << order[i].start << setw(10) << order[i].duration <<  setw(10) << order[i].reward <<  endl;
    }
    cout << "Total Reward: " << reward << endl;
    
    cout << map.size() <<endl;
    
    
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double, std::milli> time_span = t2 - t1;
    std::cout << "It took "  << time_span.count() << " milliseconds.";
    std::cout << std::endl;
    
}

//reconstruct solution by passing copy by reference in end of function

int maxReward(list<Quest> quests, vector<Quest> &order, unordered_map<Quest, OutputObj, Hasher> &map){
    if(quests.size() == 0){
        return 0;
    }
    
    Quest current = quests.front();
    
    //finding max without current
    quests.pop_front();
    vector<Quest> noCurrentOrder;
    
    
    int maxNoCurrent = maxReward(quests, noCurrentOrder, map);
    
    //removing quests that overlap binary search here??
    while(quests.size() > 0 && quests.front().start < current.start + current.duration){
        quests.pop_front();
    }
    //finding max with current
    vector<Quest> currentOrder;
    int maxCurrent = 0;
    
    auto it = map.find(current);
    
    if(it != map.end()){
//        cout << "here" << endl;
        maxCurrent = it->second.reward;
        currentOrder = it->second.quests;
    }else{
        maxCurrent = maxReward(quests, currentOrder, map) + current.reward;
        OutputObj obj;
        obj.reward = maxCurrent;
        obj.quests = currentOrder;
        map[current] = obj;
    }
    
    //returning max
    if(maxCurrent > maxNoCurrent){
        //add members of currentOrder to order
        order.push_back(current);
        for(int i = 0; i < currentOrder.size(); i++){
            order.push_back(currentOrder[i]);
        }

        return maxCurrent;
    }
    
    for(int i = 0; i < noCurrentOrder.size(); i++){
        order.push_back(noCurrentOrder[i]);
    }
    
    return maxNoCurrent;
    
}
