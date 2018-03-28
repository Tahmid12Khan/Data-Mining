#include<bits/stdc++.h>
using namespace std;

map <string, int> spam, notSpam;
vector <string> spamMsg, notSpamMsg;
vector <string> test;
set <string> words;
string fileName = "train.txt";
int totalSpamCount, totalHamCount;

void readString(string &s){
    if(s[0] == 's')
        spamMsg.push_back(s);
    else if(s[0] == 'h')notSpamMsg.push_back(s);
   // if(v.)
}

void updateString(string &s){
    for(int i = 0; i < s.size(); i++){
        if(s[i] >= 65 and s[i] <= 91)s[i] += 32;
        else if(s[i] >= 97 and s[i] <= 122){}
        else s[i] = ' ';
    }
}

void readFile(){
    ifstream name(fileName);
    string s;
    while(getline(name, s)){
        readString(s);
    }
}

void divide(){
    int testSpamSize = spamMsg.size() / 10;  //take 10% spam and 10% ham data randomly for testing and rest will be used for training

    int testHamSize = notSpamMsg.size() /10;

    srand(time(NULL));
    while(testSpamSize){
        int val = (rand() * rand()) % spamMsg.size();
        //cout << val << " " << spamMsg.size() << endl;
        test.push_back(spamMsg[val]);
        spamMsg.erase(spamMsg.begin() + val);
        testSpamSize--;
    }

     while(testHamSize){
        int val = (rand() * rand()) % notSpamMsg.size();
        test.push_back(notSpamMsg[val]);
        notSpamMsg.erase(notSpamMsg.begin() + val);
        testHamSize--;
    }

}


void updateString(){
    for(int i = 0; i < spamMsg.size(); i++){
        for(int j = 0; j < 4; j++){
            spamMsg[i][j] = ' ';
        }
        updateString(spamMsg[i]);
    }
    for(int i = 0; i < notSpamMsg.size(); i++){
        for(int j = 0; j < 3; j++){
            notSpamMsg[i][j] = ' ';
        }
        updateString(notSpamMsg[i]);
    }
}


void process(){
    for(int i = 0; i < spamMsg.size(); i++){
        istringstream iss(spamMsg[i]);
        string s;
        while(iss >> s){
            totalSpamCount++;
            spam[s]++;
            words.insert(s);
        }
    }

    for(int i = 0; i < notSpamMsg.size(); i++){
        istringstream iss(notSpamMsg[i]);
        string s;
        while(iss >> s){
            totalHamCount++;
            notSpam[s]++;
            words.insert(s);
        }
    }
}

double pSpam, pHam;

double pWord(string &s, bool isSpam){
    int alpha = 1;
    if(isSpam){
     //   cout << s << " " << spam[s] << " ss " << totalSpamCount << " " << words.size() << endl;
        return (double) log((spam[s] + alpha * 1.0)  / (totalSpamCount + words.size()) * 1.0);
    }
    return (double) log((notSpam[s] + alpha * 1.0) / (totalHamCount + words.size()) * 1.0);
}

int res[2][2];

void testing(){
    int correct = 0;
    for(int i = 0; i < test.size(); i++){
        int realResult = 0;
        cout << "Test Case #" << i + 1 << endl;
       // cout << "Message: " << test[i] << endl;
        if(test[i][0] == 's'){
            for(int j = 0; j < 4; j++)test[i][j] = ' ';
            realResult = 1;
        }
        else{
           for(int j = 0; j < 3; j++)test[i][j] = ' ';
        }
        updateString(test[i]);
        istringstream iss(test[i]);
        string s;
        double pS = log(pSpam), pH = log(pHam);
      //  double pS = 0.0, pH = 0.0;
        while(iss >> s){
            pS += pWord(s, 1);// + log(pSpam);
       //     cout << fixed << setprecision(10) << pWord(s, 1) << " " << pWord(s, 0) << endl;
            pH += pWord(s, 0);// + log(pSpam);
        }
       // cout << "---------------------------------\n";
        //cout << fixed << setprecision(10) << pS << " " << pH << endl;
        if(pS * 1.0 / (pS + pH) < .495){
            cout << "Decision -> Spam\n";
            if(realResult)cout << "Real Result -> Spam\n";
            else cout << "Real Result -> Ham\n";
            res[1][realResult]++;
            if(1 == realResult)correct++;
        }
        else{
            cout << "Decision -> Ham\n";
            if(realResult)cout << "Real Result -> Spam\n";
            else cout << "Real Result -> Ham\n";
            res[0][realResult]++;
            if(0 == realResult)correct++;
        }
        cout << "**********************************\n";
    }
    cout << "Total Test Cases: " << test.size() << endl;
    cout << "Total Correct Decisions: " << correct << endl;

}

void getMetrics(){
    string a[2][2];
    a[1][1] = "True Positive (tp)";
    a[0][0] = "True Negative (tn)";
    a[1][0] = "False Positive (fp)";
    a[0][1] = "False Negative (fn)";

    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 2; j++){
            cout << a[i][j] << ": " << res[i][j] << endl;
        }
    }
    cout << "Precision: ";
    cout << fixed << setprecision(6) << (double) res[1][1] / (res[1][1] + res[1][0]) << endl;
    cout << "Recall: ";
    cout << fixed << setprecision(6) << (double) res[1][1] / (res[1][1] + res[0][1]) << endl;
    cout << "Accuracy: ";
    cout << fixed << setprecision(6) << (double) (res[1][1] + res[0][0])/ (res[1][1] + res[1][0] + res[0][0] + res[0][1]) << endl;
    cout << "F-measure: ";
    cout << fixed << setprecision(6) << (double) (2.0 * res[1][1])/ (2 * res[1][1] + res[1][0] + res[0][1]) << endl;
}


int main(){
    readFile();
    cout << "File Reading Completed\n";
    cout << "Total Spam messages: " << spamMsg.size() << endl;
    cout << "Total Ham messages: " << notSpamMsg.size() << endl;
    divide();
    cout << "Train and Test data have been divided\n";
    updateString();
    process();
    pSpam = (double) (spamMsg.size())/ (spamMsg.size() + notSpamMsg.size()) * 1.0;
    pHam = (double) 1 - pSpam;
    testing();
    getMetrics();


}
