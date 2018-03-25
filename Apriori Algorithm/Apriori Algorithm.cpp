#include <bits/stdc++.h>
using namespace std;

map <vector <string>, int> counts, temporaryCount, combinations, lastCombinations, combinationsWithoutFiltering, invalidCombinations;
vector < vector <string> > cnt;
string fileName = "input.txt";
int supportCount = 2;
void print(){
    cout << "-----------------------------------\n";
}
void printVector(vector <string> &v){
    for(int i = 0; i < v.size(); i++){
        cout << v[i] << " ";
    }
}

void print(map <vector <string>, int > &combinations){
    for(auto it = combinations.begin(); it != combinations.end(); it++){
        vector <string> temp = it -> first;
        cout << "{ ";
        printVector(temp);
        cout << "}";
        cout << " -> " << temporaryCount[it -> first] << endl;
    }
    print();
}

bool subsequenceMatch(vector <string> &v1, vector <string> &v2){
    if(v1.size() > v2.size())
        return false;
    int j = 0;
    for(int i = 0; i < v2.size(); i++){
        if(v1[j] == v2[i])j++;
        if(j == v1.size())break;
    }
    return (j == v1.size() );
}

int getMaxCol(vector <vector <string> > &cnt){
    int mx = 0;
    for(int i = 0; i < cnt.size(); i++){
        int length = cnt[i].size();
        mx = max(mx, length);
    }
    return mx;
}

void checkAllValidCombinationOfLengthNMinusOneAndFilter(int i, vector <string> &NLen, map <vector <string>, int> &lastCombinations, vector <string> &NMinusOneLen, bool &valid){
    if(i == NLen.size()){
        if(NMinusOneLen.size() == NLen.size() - 1 and lastCombinations.find(NMinusOneLen) == lastCombinations.end()){
            valid = false; //If any N - 1 len combination is not present in last combination we ignore this combination of len N
        }
        return;
    }
    if(valid){
        NMinusOneLen.push_back(NLen[i]);
        checkAllValidCombinationOfLengthNMinusOneAndFilter(i + 1, NLen, lastCombinations, NMinusOneLen, valid);
        NMinusOneLen.erase(NMinusOneLen.end() - 1);
        checkAllValidCombinationOfLengthNMinusOneAndFilter(i + 1, NLen, lastCombinations, NMinusOneLen, valid);
    }
}

//If you want to enter new combination based on prefix (but common matcher is preferred as it does not miss any combination)
/*
int prefixMatch(vector <string> &v1, vector <string> &v2){
    int matched = 0;

    for(int i = 0; i < v2.size() and i < v1.size(); i++){
        if(v1[i] == v2[i])matched++;
        else break;
    }
    return matched;
}

void enterNewCombination(vector <string> &v1, vector <string> &v2, map <vector <string>, int > &combinations){
    vector <string> temp = v1;
    temp.push_back(v2[v2.size() - 1]);
    sort(temp.begin(), temp.end());
    if(invalidCombinations.find(temp) != invalidCombinations.end())return;
    vector <string> NLenMinusOne;
    combinationsWithoutFiltering[temp]++;
    bool valid = true;
    checkAllValidCombinationOfLengthNMinusOneAndFilter(0, temp, lastCombinations, NLenMinusOne, valid);
    if(valid){
       combinations[temp]++;
    }
    else{
        invalidCombinations[temp]++;
    }
}
*/


int commonMatch(vector <string> &v1, vector <string> &v2){
    int matched = 0;
    set <string> s;
    for(int i = 0; i < v1.size(); i++){
        s.insert(v1[i]);
    }
    for(int i = 0; i < v2.size(); i++){
        if(s.find(v2[i]) != s.end())matched++;
        //else s.insert(v2[i]);
    }
    s.clear();
    return matched;
}




void enterNewCombination(vector <string> &v1, vector <string> &v2, map <vector <string>, int > &combinations){
    vector <string> temp;
    set <string> s;
    for(int i = 0; i < v1.size(); i++){
        s.insert(v1[i]);
    }
    for(int i = 0; i < v2.size(); i++){
        s.insert(v2[i]);
    }
    for(auto it = s.begin(); it != s.end(); it++){
        temp.push_back(*it);
    }
    if(invalidCombinations.find(temp) != invalidCombinations.end())return;
    vector <string> NLenMinusOne;
    combinationsWithoutFiltering[temp]++;
    bool valid = true;
    checkAllValidCombinationOfLengthNMinusOneAndFilter(0, temp, lastCombinations, NLenMinusOne, valid);
    if(valid){
       combinations[temp]++;
    }
    else{
        invalidCombinations[temp]++;
    }
}

void getSingleCombination(vector <vector <string> > &cnt, map <vector <string>, int > &combinations){
  for(int i = 0; i < cnt.size(); i++){
        vector <string> temp = cnt[i];
        vector <string> temp2;
        for(int j = 0; j < temp.size(); j++){
            temp2.push_back(temp[j]);
            combinations[temp2]++;
            temp2.clear();
        }
    }
}

void getCombinations(map <vector <string>, int > &lastCombinations, map <vector <string>, int > &combinations, int commonCount){
    for(auto it = lastCombinations.begin(); it != lastCombinations.end(); it++){
        vector <string> temp = it -> first;
        //printVector(temp);
        //cout << "\nxxxxxxxxxxxxxxxxxxx\n";
        for(auto it2 = lastCombinations.begin(); it2 != lastCombinations.end(); it2++){
            vector <string> temp2 = it2 -> first;
            if(commonMatch(temp, temp2) == commonCount - 2){
                enterNewCombination(temp, temp2, combinations);
            }
        }
    }

}


void getcounts(vector <vector <string> > &cnt, vector <string> &given, map <vector <string>, int > &temporaryCount){
    for(int i = 0; i < cnt.size(); i++){
        if(subsequenceMatch(given, cnt[i])){
            temporaryCount[given]++;
        }
    }
}

void getAllCombinationCount(vector <vector <string> > &cnt, map <vector <string>, int > &combinations, map <vector <string>, int > &temporaryCount){
    for(auto it = combinations.begin(); it != combinations.end(); it++){
        vector <string> temp = it -> first;
        getcounts(cnt, temp, temporaryCount);
    }
}

void lessThanSupportCount(map <vector <string>, int > &temporaryCount, int val){
    for(auto it = temporaryCount.begin(); it != temporaryCount.end();  it++){
        if(it -> second < val){
            combinations.erase(it -> first);
        }
    }
}



void clearString(string &s){
    string toClean = ",.!@#$%^&*():;><|`~+-*";
    for(int i = 0; i < s.size(); i++){
        for(int j = 0; j < toClean.size(); j++){
            if(s[i] == toClean[j]){
                s[i] = ' ';
                break;
            }
        }
    }
}

void parseInput(string s){
    clearString(s);
    istringstream iss(s);
    string temp;
    iss >> temp; //get rid of id
    vector <string> nums;
    string num;
    while(iss >> num){
        nums.push_back(num);
    }
    //printVector(nums);
    //cout << endl;
    cnt.push_back(nums);
}

void readFile(){
    ifstream open_file(fileName);
    string s;
    while(getline(open_file, s)){
        parseInput(s);
        //cout << s << endl;

    }
}

void savecounts(map <vector <string>, int> &counts, map <vector <string>, int> &combinations, map <vector <string>, int> &temporaryCount){

    for(auto it = combinations.begin(); it != combinations.end(); it++){
        counts[it -> first] = temporaryCount[it -> first];

    }
}

void printSaveCounts(map <vector <string>, int>  counts){
    for(auto it = counts.begin(); it != counts.end(); it++){
        cout << "{ ";
        vector <string> temp = it -> first;
        printVector(temp);
        cout << "}";
        cout << " -> " << it -> second << endl;
    }
}

void queryParse(vector <string> &v){
    string s;
    getline(cin, s);
    istringstream iss(s);
    while(iss >> s){
        v.push_back(s);
    }
}

void printAnswer(vector <string> &temp, vector <string> &temp2, vector <string> &temp3, map < vector <string>, int> &counts){
    printVector(temp);
    cout << "=>";
    printVector(temp3);
    cout << ": ";
    cout << fixed << setprecision(10) << (double) counts[temp2] / counts[temp] * 1.0 << endl;
    print();
}

void mergeVectors(vector <string> &v1, vector <string> &v2){
    for(int i = 0; i < v1.size(); i++){
            v2.push_back(v1[i]);
        }
}

void query(map <vector <string>, int>  &counts){
    while(true){
        vector <string> temp, temp2, temp3;
        cout << "Enter 1st condition separated by spaces: ";
        queryParse(temp);
        sort(temp.begin(), temp.end());
        cout << "Enter 2nd condition separated by spaces: ";
        queryParse(temp2);
        temp3 = temp2;
        sort(temp3.begin(), temp3.end());
        mergeVectors(temp, temp2);
        sort(temp2.begin(), temp2.end());
        if(counts[temp2] == 0 or counts[temp] == 0){
            cout << "Either 1st or 2nd condition is less than the support count\n";
            continue;
        }
        printAnswer(temp, temp2, temp3, counts);
    }
}

void printcombinationsWithoutFiltering(map <vector <string>, int > &combinations){
    for(auto it = combinations.begin(); it != combinations.end(); it++){
        vector <string> temp = it -> first;
        cout << "{ ";
        printVector(temp);
        cout << "}";
        cout << endl;
    }
    print();
}

void showTable(){
    cout << "\nCombinations without filtering\n";
    print();
    printcombinationsWithoutFiltering(combinationsWithoutFiltering);

    cout << "\nInvalid Combinations\n";
    print();
    printcombinationsWithoutFiltering(invalidCombinations);

    cout << "\n\nNormal Count (Combinations after filtering)\n";
    print();
    print(combinations);
    lessThanSupportCount(temporaryCount, supportCount);

    cout << "\n-----Filtering By Support Count----\n";
    print();
    print(combinations);

    lastCombinations.clear();
    lastCombinations = combinations;
    savecounts(counts, combinations, temporaryCount);
    combinations.clear();
    temporaryCount.clear();
    combinationsWithoutFiltering.clear();
    invalidCombinations.clear();
}



int main(){
    readFile();
    int max_col = getMaxCol(cnt);
    getSingleCombination(cnt, combinations);
    temporaryCount = combinations;
    combinationsWithoutFiltering = combinations;
    showTable();

    for(int i = 2; i <= max_col; i++){

        getCombinations(lastCombinations, combinations, i);

        getAllCombinationCount(cnt, combinations, temporaryCount);
        showTable();
        if(lastCombinations.size() == 0){
            cout << "\nNo valid combination of length " << i << " remains\n";
            break;
        }
    }

    cout << "\n\nSaved Data:\n";
    print();
    printSaveCounts(counts);
    query(counts);

}
