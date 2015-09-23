#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

const string ALPHABET  = "abcdefghijklmnopqrstuvwxyz";
//const string dictFilePath = "/Users/Daniel/Documents/Programmering/TDDD86/Lab2/evilhangman/res/dictionary.txt";
const string dictFilePath = "dictionary.txt";

const string ANSWER_YES = "yes";
const string ANSWER_NO = "no";

map<int, vector<string> > loadDictionary() {
    map<int, vector<string> > dictMap;
    ifstream stream;
    string word;
    stream.open(dictFilePath);

    if(!stream.is_open()){
        cout << "Unable to find file" << endl;
    }

    while (!stream.eof()) {
        stream >> word;
        dictMap[word.length()].push_back(word);
    }
    return dictMap;
}

//kopia??
pair<string,vector<string> > divideWordFamilies(char guessedLetter,int nmGuesses,int wordLength, string wordToGuess, vector<string>& words){
    map<string, vector<string> > families;
    int occurences = -1;

    for(string word : words){
        string keyString = wordToGuess;
        occurences = std::count(word.begin(),word.end(),guessedLetter);

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


    if(nmGuesses == 1 && families[wordToGuess].size() > 0){
        pair<string, vector<string> > returnedPair (wordToGuess, families[wordToGuess]);
        return returnedPair;
    }


    unsigned int sizeOfBiggest = 0;
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

        pair<string, vector<string> > family = divideWordFamilies(guessedLetter, nmGuesses,wordLength,wordToGuess,words);
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
    cout << "Welcome to Hangman." << endl;

    bool playAgain = true;
    map<int, vector<string> > dictMap = loadDictionary();

    while(playAgain){

        /* Get the length of the word to guess */

        int wordLength= -1;
        while (dictMap.find(wordLength) == dictMap.end()) {
            cout << "Please enter the valid length of a word: ";
            cin >> wordLength;
        }

        /* Get number of guesses */

        int nmGuesses = -1;
        while (nmGuesses <= 0) {
            cout << "Type the number of guesses that you want: ";
            cin >> nmGuesses;
        }

        /* Get if user wants to see number of words left */

        bool showWordCount = false;
        string inputUserChoice = "";
        while (inputUserChoice != ANSWER_YES && inputUserChoice != ANSWER_NO) {
            cout << "Do you want to see the number of words left after each guess? (" << ANSWER_YES << "/" << ANSWER_NO << ") ";
            cin >> inputUserChoice;
        }
        if (inputUserChoice == ANSWER_YES) {
            showWordCount = true;
        }

        /* Run game */

        runGame(showWordCount, nmGuesses, wordLength, dictMap[wordLength]);

        /* Get if user wants to play again */

        string playAgainInput = "";
        cout <<  endl << "Do you want to play again? (" << ANSWER_YES << "/" << ANSWER_NO << ") ";
        cin >> playAgainInput;
        playAgain = (playAgainInput == ANSWER_YES);
    }

    return 0;
}
