/* This project produces a personality quiz, it starts off by opening two given files
one with a set of questions and the other with users and their inputs.
The personality quiz built is based on a five factor model given by numbers in five different catefories based
off the acronym 'OCEAN': Openness, conscientiousness, extraversion, agreeableness, neuroticism.
As each question is answered based off of user input the program updates scores into each category and at the end assigns a fictional character whose scores are most similar to the user.*/

#include <set>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "driver.h"

using namespace std;
void dumpMap(map<char, int> m1) {
  for (auto a : m1) {
    cout << " " << a.first << "->" << a.second << endl;
  }
}

void dumpSetQ(set<Question> question) {
  cout << "Set Q" << endl;
  for (Question x : question) {
    cout << " " << x.questionText << endl;
    dumpMap(x.factors);
  }
}

// a function that checks, opens and parses through the question file and stores
void checkQuestions(set<Question> &questions) {
  string toParse;
  ifstream fileName("questions.txt");

  while (getline(fileName, toParse)) {
    Question storeQues;
    // parses to find the first question to the period (where it ends)
    storeQues.questionText = toParse.substr(0, toParse.find(".")+1);
    // parses to find after the question ends to the character and value along
    string line;
    stringstream parser(toParse.substr(toParse.find(".")+2));
    // converts value from string to int
    while (getline(parser, line, ' ')) {
      storeQues.factors[line[0]] = stoi(line.substr(2));
    }
    questions.insert(storeQues);  // inserts the stored question into questions
  }
}

// a function to open the character files and checks, opens,
// and parses through them and stores.
void checkCharacters(vector<set<Person>> &characters) {
  vector<string> fileNames = {"BabyAnimals.people", "Brooklyn99.people",
                            "Disney.people", "Hogwarts.people",
                            "MyersBriggs.people", "SesameStreet.people",
                            "StarWars.people", "Vegetables.people",
                            "LooneyTunes.people"};
  // for each file through the filenames
  for (string file : fileNames) {
    ifstream fileTypes(file);
    string line;
    string toParse;
    set<Person> users;  // stores users
    while (getline(fileTypes, line)) {
      Person charac;  // characters
      charac.name = line.substr(0, line.find("."));  // until name is over
      stringstream parse(line.substr(line.find(".")+2));  // after decimal & space
      // parses through file and converts the number from a string to an integer
      while (getline(parse, toParse, ' ')) {
        charac.scores[toParse[0]] = stoi(toParse.substr(2));
      }
      users.insert(charac);  // inserts the characters into the users
    }
    characters.push_back(users);  // pushes the users into the characters vector
  }
}

/* this function produces the user interface questions based off of the
  number of questions the user selected, and stores the user answers into inputs for each selected question
*/
void userInterfaceQues(int &numQues, set<Question> &questions,
                      vector<set<Person>> &characters,
                      map<Question, int> &inputs) {
  int answers;
  while (numQues != 0) {
    cout << "How much do you agree with this statement?" << endl;
    Question ques = randomQuestionFrom(questions);
    cout << "\"" << ques.questionText << "\"" << endl << endl;
    cout << "1. Strongly disagree" << endl << "2. Disagree" << endl << "3. Neutral"
         << endl << "4. Agree" << endl << "5. Strongly agree" << endl << endl
         << "Enter your answer here (1-5): ";
    cin >> answers;
    cout << endl;
    numQues--;  // keeps subtracting until = to 0 
    inputs[ques] = answers;  //stores the answers for the questions
  }
}

int main() {
  //variable declarization 
    set<Question> questions;
    checkQuestions(questions);
    vector<set<Person>> characters; 
    checkCharacters(characters);
    map<Question, int> inputs;
    int numQues;

    cout << "Welcome to the Personality Quiz!" << endl << endl;
    cout << "Choose number of questions: " << endl;
    cin >> numQues;  // takes user input for the number of questions
    userInterfaceQues(numQues, questions, characters, inputs);

    map<char, int> userScores = scoresFrom(inputs);

    while (true) {
      cout << "1. BabyAnimals" << endl << "2. Brooklyn99" << endl << "3. Disney"
           << endl << "4. Hogwarts" << endl << "5. MyersBriggs" << endl
           <<"6. SesameStreet" << endl << "7. StarWars" << endl
           << "8. Vegetables" << endl << "9. mine"
           << endl << "0. To end program." << endl << endl;

      int userInput;
      cout << "Choose test number (1-9, or 0 to end): ";
      cin >> userInput;
      if (userInput != 0) {
        Person similarTo = mostSimilarTo(userScores, characters[userInput-1]);
        cout << "You got " << similarTo.name << "!" << endl << endl;
      } else {
        cout << "Goodbye!" << endl;
        break;
             }
    }
    return 0;
}