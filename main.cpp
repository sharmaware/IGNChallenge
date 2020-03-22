
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
//#include <algorithm>

using namespace std;

struct quest{
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

int maxReward(list<quest> quests, int reward);

int main(int argc, const char * argv[]) {
    //using list for fast erasing from front
    list<quest> quests;
    loadQuests("/Users/saarthaksharma/Desktop/IGNCodeChallenge/IGNCodeChallenge/quests.txt", quests);
    quests.sort();
//    sort(quests.begin(), quests.end());
    
    
    for(auto it = quests.begin(); it != quests.end(); it++){
        cout << it->name << " " << it->start << " " << it->duration << " " << it->reward << " " <<it->reward/it->duration << endl;
    }

    int max = maxReward(quests, 0);
    cout << max << endl;
    
}

//create map to return values?
int maxReward(list<quest> quests, int reward/*, vector<quest> &bestOrder*/){
    if(quests.size() == 0){
        return reward;
    }
    
    quest current = quests.front();
    cout << quests.size() << " " << current.name<< endl;
    //finding max without current
    quests.pop_front();
    int maxNoCurrent = maxReward(quests, reward);
    
    //removing quests that overlap
    while(quests.size() > 0 && quests.front().start < current.start + current.duration){
        quests.pop_front();
    }
    //finding max with current
    int maxCurrent = maxReward(quests, reward+current.reward);
    
    //returning max
    if(maxCurrent > maxNoCurrent){
        return maxCurrent;
    }
    return maxNoCurrent;
    
    
}
