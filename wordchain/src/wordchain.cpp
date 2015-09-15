#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <set>
#include <vector>
using namespace std;

const string ALPHABET  = "abcdefghijklmnopqrstuvwxyz";


void loadDictionary(set<string>& dictionary) {
    string word;
    ifstream stream;
    stream.open("/Users/Daniel/Documents/Programmering/TDDD86/Lab2/wordchain/res/dictionary.txt");
    if (!stream.is_open()) {
        cout << "Could not load dictionary." << endl;
    }
    while (!stream.eof()) {
        getline(stream, word);
        dictionary.insert(word);
    }
}

vector<string> findClosestNeighbors(string& originalWord, set<string>& dictionary, set<string>& alreadyFoundWords) {
    string word;
    vector<string> foundNeighbors;
    for (unsigned long int i = 0; i < originalWord.length(); ++i) {
        word = originalWord;
        for (string::const_iterator letter = ALPHABET.begin(); letter != ALPHABET.end(); ++letter) {
            word[i] = *letter;
            if (dictionary.find(word) != dictionary.end() && alreadyFoundWords.find(word) == alreadyFoundWords.end()) {
                foundNeighbors.push_back(word);
                alreadyFoundWords.insert(word);
            }
        }
    }
    return foundNeighbors;
}

void wordChain(string& firstWord, string& secondWord, set<string>& dictionary) {
    queue<stack<string>> chainQueue;
    stack<string> firstStack;
    set<string> alreadyFoundWords;
    firstStack.push(firstWord);
    chainQueue.push(firstStack);

    while (!chainQueue.empty()) {
        auto stackToCheck = chainQueue.front();
        chainQueue.pop();
        if (stackToCheck.top() == secondWord) {
            while (!stackToCheck.empty()) {
                cout << stackToCheck.top() << " ";
                stackToCheck.pop();
            }
            cout << endl;
            return;
        } else {
            vector<string> neighbors = findClosestNeighbors(stackToCheck.top(), dictionary, alreadyFoundWords);
            for (string neighbor : neighbors) {
                stack<string> newStack (stackToCheck);
                newStack.push(neighbor);
                chainQueue.push(newStack);
            }
        }
    }
    cout << "No words found." << endl;
}

int main() {
    set<string> dictionary;
    loadDictionary(dictionary);

    cout << "Welcome to TDDD86 Word Chain." << endl;
    cout << "If you give me two English words, I will transform the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;
    cout << "Number of dictionary words: " << dictionary.size() << endl;

    cout << "Please type two words: ";
    string firstWord, secondWord;
    cin >> firstWord >> secondWord;

    while (firstWord != "exit") {
        wordChain(firstWord, secondWord, dictionary);
        cout << "Please type two words: ";
        cin >> firstWord >> secondWord;
    }

    // TODO: Finish the program!

    return 0;
}
