
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <unordered_map>

using namespace std;

class quest{
public:
    string name;
    int start;
    int duration;
    int reward;
    string difficulty;
    string location;
    string giver;
    
    bool operator< (const quest &other) const {
        if(start < other.start){
            return true;
        }
        return false;
    }
    
};

void loadQuests(string path, list<quest> &quests){
    ifstream infile (path);
    
    while(true){
        quest q;
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

int maxReward(list<quest> quests, int reward, vector<quest> &order);

int main(int argc, const char * argv[]) {
    //using list for fast erasing from front
    list<quest> quests;
    loadQuests("/Users/saarthaksharma/Desktop/IGNCodeChallenge/IGNCodeChallenge/quests.txt", quests);
    quests.sort();
//    sort(quests.begin(), quests.end());
    
    
    for(auto it = quests.begin(); it != quests.end(); it++){
        cout << it->name << "         " << it->start << "      " << it->duration << "       " << it->reward << "       " <<it->reward/it->duration << endl;
    }

    vector<quest> order;
    int max = maxReward(quests, 0, order);
    cout << max << endl;
    
    cout << order.size() << endl;
    
    for(int i = 0; i < order.size(); i++){
        cout << order[i].name << " " << order[i].start << " " << order[i].duration <<  " " << order[i].reward <<  endl;
    }
    
}

//create map to return values?
//reconstruct solution by passing copy by reference in end of function

int maxReward(list<quest> quests, int reward, vector<quest> &order){
    if(quests.size() == 0){
        return reward;
    }
    
    quest current = quests.front();
    
    //finding max without current
    quests.pop_front();
    vector<quest> noCurrentOrder;
    int maxNoCurrent = maxReward(quests, reward, noCurrentOrder);
    
    //removing quests that overlap binary search here??
    while(quests.size() > 0 && quests.front().start < current.start + current.duration){
        quests.pop_front();
    }
    //finding max with current
    vector<quest> currentOrder;
    int maxCurrent = maxReward(quests, reward+current.reward, currentOrder);
    
    //returning max
    if(maxCurrent > maxNoCurrent){
        //add members of currentOrder to order
        for(int i = 0; i < currentOrder.size(); i++){
            order.push_back(currentOrder[i]);
        }
        order.push_back(current);
        
        return maxCurrent;
    }
    
    for(int i = 0; i < noCurrentOrder.size(); i++){
        order.push_back(noCurrentOrder[i]);
    }
    
    return maxNoCurrent;
    
}
