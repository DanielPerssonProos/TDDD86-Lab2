#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

const string ALPHABET  = "abcdefghijklmnopqrstuvwxyz";
const string dictFilePath = "/Users/Daniel/Documents/Programmering/TDDD86/Lab2/evilhangman/res/dictionary.txt";

map<int, vector<string> > loadDictionary() {
    map<int, vector<string> > dictMap;
    ifstream stream;
    string word;
    stream.open(dictFilePath);
    while (!stream.eof()) {
        stream >> word;
        dictMap[word.length()].push_back(word);
    }
    return dictMap;
}

pair<string,vector<string> > divideWordFamilies(char guessedLetter,int wordLength, string wordToGuess, vector<string>& words){
    map<string, vector<string> > families;
    int occurences = -1;

    for(string word : words){
        string keyString = wordToGuess;
        occurences = count(word.begin(),word.end(),guessedLetter);

        if(occurences == 1){
            int letterIndex = word.find(guessedLetter);
            keyString.at(letterIndex) = guessedLetter;
        }else if(occurences == 2){
            int firstLetterIndex = word.find(guessedLetter);
            int lastLetterIndex = word.find_last_of(guessedLetter);

            keyString.at(firstLetterIndex) = guessedLetter;
            keyString.at(lastLetterIndex) = guessedLetter;
        }else if(occurences > 2){

            for(int i = 0; i < wordLength;++i){
                if(word.at(i) == guessedLetter){
                    keyString.at(i) = guessedLetter;
                }
            }

        }

        families[keyString].push_back(word);
    }


    int sizeOfBiggest = 0;
    pair<string, vector<string> > biggestFamily;

    for(auto iterator = families.begin(); iterator != families.end(); ++iterator){
        if((*iterator).second.size() > sizeOfBiggest){
            sizeOfBiggest = (*iterator).second.size();
            biggestFamily = *iterator;
        }
    }
    return biggestFamily;
}

void runGame(bool& showWordCount, int& nmGuesses, int wordLength, vector<string>& words) {
    string prevGuessedLetters = "";
    string wordToGuess = string(wordLength, '-');
    char guessedLetter;


    bool gameEnded = false;
    while(!gameEnded) {

        int wordsLeft = words.size();
        cout << "You have " << nmGuesses << " guesses left." << endl
             << "Guessed letters: " << prevGuessedLetters << endl;
        if (showWordCount) {
            cout << "Number of words remaining: " << wordsLeft << endl;
        }
        while (true) {
            string placeHolder = "";
            cout << "Guess a letter: ";
            cin >> placeHolder;
            if (placeHolder.length() == 1 && ALPHABET.find(placeHolder) != string::npos
                    && prevGuessedLetters.find(placeHolder) == string::npos) {
                prevGuessedLetters += placeHolder;
                guessedLetter = placeHolder[0];
                break;
            }
        }

        pair<string, vector<string> > family = divideWordFamilies(guessedLetter,wordLength,wordToGuess,words);
        if (family.first == wordToGuess) {
            --nmGuesses;
        }
        wordToGuess = family.first;
        words = family.second;
        if (nmGuesses == 0) {
            cout << "You lost." << endl
                 <<"The correct word was: " << words[0] << endl;
            gameEnded = true;
        } else if (wordToGuess.find('-') == string::npos) {
            cout << "You won! You correctly guessed the word " << wordToGuess  << endl;
            gameEnded = true;
        }else{
            cout << endl << endl << wordToGuess << endl << endl;


        }
    }
}




int main() {
    int nmGuesses = -1;
    int wordLength= -1;
    string placeHolder = "";
    bool showWordCount = false;
    cout << "Welcome to Hangman." << endl;
    map<int, vector<string> > dictMap = loadDictionary();
    while (dictMap.find(wordLength) == dictMap.end()) {
        cout << "Please enter the valid length of a word: ";
        cin >> wordLength;
    }
    while (nmGuesses <= 0) {
        cout << "Type the number of guesses that you want: ";
        cin >> nmGuesses;
    }
    while (placeHolder != "y" && placeHolder != "n") {
        cout << "Do you want to see the number of words left after each guess? (y/n) ";
        cin >> placeHolder;
    }
    if (placeHolder == "y") {
        showWordCount = true;
    }
    runGame(showWordCount, nmGuesses, wordLength, dictMap[wordLength]);



    // TODO: Finish the program!

    return 0;
}
