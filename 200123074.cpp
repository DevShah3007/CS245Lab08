#include <bits/stdc++.h>
using namespace std;
vector<pair< pair< int, vector<int> >, pair< int,int > >*> bucketList;

// pair< pair< int, vector<int> >, pair< int,int > >
// pair< pair< time, vector<int>>, pair<localdepth,numberofkeys>>

bool comparator( pair< pair< int, vector<int> >, pair< int,int > >* p1,  pair< pair< int, vector<int> >, pair< int,int > >* p2)
{
    return p1->first.first < p2->first.first;
}

int power2(int x){
    int res = 1;
    for(int i=1; i<=x; i++){
        res*=2;
    }
    return res;
}

int currentGlobalDepth;
int Time = 1;


void Insert(vector< pair< pair< int, vector<int> >, pair< int,int > > *> &drt, int value, int maxSize){
    int idx = value % power2(currentGlobalDepth);
    if(drt[idx]->second.second < maxSize){
        drt[idx]->first.second.push_back(value);
        drt[idx]->second.second++;
        // cout << "Inserted " << value <<'\n';
    }else if(drt[idx]->second.first < currentGlobalDepth){
        drt[idx]->second.first++;
        int ld = drt[idx]->second.first;
        for(int i=0; i<drt.size(); i++){
            if(drt[i] == drt[idx] && i!=idx && idx%power2(ld) != i%power2(ld)){
                drt[i] = new pair< pair< int, vector<int> >, pair< int,int > >;
                drt[i]->first.first = Time;
                ++Time;
                drt[i]->second.first = drt[idx]->second.first;
                drt[i]->second.second = 0;
                bucketList.push_back(drt[i]);
            }
        }
        int vsize = drt[idx]->first.second.size();
        for(int i=0; i<vsize; i++){
            int dummyidx = drt[idx]->first.second[i] % power2(currentGlobalDepth);
            if(drt[dummyidx] != drt[idx]){
                // drt[idx]->first.second.erase(drt[idx]->first.second.begin()+i);
                int temp = drt[idx]->first.second[i];
                for(int j=i+1; j<vsize; j++){
                    drt[idx]->first.second[j-1] = drt[idx]->first.second[j];
                }
                drt[idx]->second.second--;
                drt[idx]->first.second.pop_back();
                i--;
                // drt[idx]->first.second.shrink_to_fit();
                vsize = drt[idx]->first.second.size();
                Insert(drt,temp,maxSize);
            }
        }
        Insert(drt,value,maxSize);
    }else{
        ++currentGlobalDepth;
        if(currentGlobalDepth<=20){
            int cd = power2(currentGlobalDepth);
            drt.resize(cd);
            for(int i=0; i<cd/2; i++){
                drt[i + cd/2] = drt[i];
            }
            drt[idx]->second.first++;
            for(int i=0; i<drt.size(); i++){
                if(drt[i] == drt[idx] && i!=idx && idx%(cd) != i%(cd)){
                    drt[i] = new pair< pair< int, vector<int> >, pair< int,int > >;
                    drt[i]->first.first = Time;
                    ++Time;
                    drt[i]->second.first = drt[idx]->second.first;
                    drt[i]->second.second = 0;
                    bucketList.push_back(drt[i]);
                }
            }
            int vsize = drt[idx]->first.second.size();
            for(int i=0; i<vsize; i++){
                int dummyidx = drt[idx]->first.second[i] % cd;
                if(drt[dummyidx] != drt[idx]){
                    //drt[idx]->first.second.erase(drt[idx]->first.second.begin()+i);
                    int temp = drt[idx]->first.second[i];
                    for(int j=i+1; j<vsize; j++){
                        drt[idx]->first.second[j-1] = drt[idx]->first.second[j];
                    }
                    drt[idx]->second.second--;
                    drt[idx]->first.second.pop_back();
                    i--;
                    // drt[idx]->first.second.shrink_to_fit();
                    vsize = drt[idx]->first.second.size();
                    Insert(drt,temp,maxSize);
                }
            }
            Insert(drt,value,maxSize);
        }else{
            cout << "Global Depth exceeded the maximum limit of 20! Could not insert "<< value <<'\n';
        }       
    }
}


bool Search(vector< pair< pair< int, vector<int> >, pair< int,int > > *> drt, int value){
    int idx = value % power2(currentGlobalDepth);
    vector<int> dummy = drt[idx]->first.second;
    if(find(dummy.begin(),dummy.end(),value) == dummy.end()){
        return false;
    }
    return true;
}

void Delete(vector< pair< pair< int, vector<int> >, pair< int,int > > *> drt, int value){
    bool deleted = false;
    int idx = value % power2(currentGlobalDepth);
    for(int i=0; i<drt[idx]->first.second.size(); i++){
        if(drt[idx]->first.second[i]==value){
            for(int j=i+1; j<drt[idx]->first.second.size(); j++){
                drt[idx]->first.second[j-1] = drt[idx]->first.second[j];
            }
            drt[idx]->first.second.pop_back();
            // drt[idx]->first.second.shrink_to_fit();
            drt[idx]->second.second--;
            cout << "Deleted " << value << '\n';
            deleted = true;
            break;
        }
    }
    if(!deleted){
        cout << "Value " << value << " not present. Deletion trivial !\n";
    }
}


void Display(vector< pair< pair< int, vector<int> >, pair< int,int > > *> drt){
    cout << currentGlobalDepth <<'\n';
    cout << bucketList.size() << '\n';
    for(auto it : bucketList){
        cout << "Number of keys : " << it->second.second << ", Local Depth : " << it->second.first << '\n';
        for(auto j : it->first.second){
            cout << j << " ";
        }
        cout << '\n';
    }
}


int main() {
   #ifndef ONLINE_JUDGE
       freopen("i.txt", "r", stdin);
        //freopen("output1.txt", "w", stdout);
   #endif // ONLINE_JUDGE 

    int globalDepth, maxBucketSize;
    cin >> globalDepth >> maxBucketSize;
    currentGlobalDepth = globalDepth;
    int initialDirectorySize = power2(globalDepth);

    vector< pair< pair< int, vector<int> >, pair< int,int > > *> directory(initialDirectorySize);

    for(int i=0; i<initialDirectorySize; i++){
        directory[i] = new pair< pair< int, vector<int> >, pair< int,int > >;
        directory[i]->second.first = globalDepth;
        directory[i]->second.second = 0;
        directory[i]->first.first = Time;
        ++Time;
        bucketList.push_back(directory[i]);
    }

    while(true){
        int opCode,number;
        cin >> opCode;
        if(opCode <=4 && opCode >=2){
            cin >> number;
            if(opCode==2){
                Insert(directory,number,maxBucketSize);
            }else if(opCode == 3){
                bool found = Search(directory,number);
                if(found){
                    cout << "Found " << number<<'\n';
                }else{
                    cout << "Not found " << number << '\n';
                }
            }else{
                Delete(directory,number);
            }
        }else if(opCode == 5){
            Display(directory);
        }else if(opCode == 6){
            cout << "Program over\n";
            break;
        }else{
            cout << "\nIncorrect operation\n";
            break;
        }
    }
    return 0;
}