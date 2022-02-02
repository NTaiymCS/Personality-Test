// This is the driver.h file created with functions to help user interface
// in main file.
#include <math.h>
#include <limits>
#include <string>
#include <utility>  // according to submission
#include <map>
#include <set>
#include "myrandom.h"
using namespace std;

constexpr double lowest_double = std::numeric_limits<double>::lowest();

/* Type: Question
 *
 * Type representing a personality quiz question.
 */
struct Question {
    string questionText;  // Text of the question
    map<char, int> factors;   // Map from factors to +1 or -1
    friend bool operator< (const Question& lhs, const Question& rhs) {
        return lhs.questionText < rhs.questionText;
    }
    friend bool operator== (const Question& lhs, const Question& rhs) {
        return lhs.questionText == rhs.questionText;
    }
    friend bool operator!= (const Question& lhs, const Question& rhs) {
        return lhs.questionText != rhs.questionText;
    }
};

/* Type: Person
 *
 * Type representing a person, used to represent people when determining
 * who's the closest match to the user.
 */
struct Person {
    string name;      // Name of the person
    map<char, int> scores;  // Map from factors to +1 or -1
    friend bool operator< (const Person& lhs,   const Person& rhs) {
        return lhs.name < rhs.name;
    }
    friend bool operator== (const Person& lhs, const Person& rhs) {
        return lhs.name == rhs.name;
    }
    friend bool operator!= (const Person& lhs, const Person& rhs) {
        return lhs.name != rhs.name;
    }
};

/* randomElement
 *
 * This function selects, at random, a Question from the inputted questions set
 * and returns the question.  Note, this function does not remove the randomly
 * selected question from the set.
*/
Question randomElement(set<Question>& questions) {
  int ind = randomInteger(0, (int)questions.size()-1);
  int i = 0;
  for (auto e : questions) {
      if (i == ind) {
          return e;
      }
      i++;
  }
  return {};
}

// ms1
/* This function chooses a random question from the given file of questions and
   once a question is selected it erases it from the list, if the the size 
   is invalid then a runtime error is thrown
*/
Question randomQuestionFrom(set<Question>& questions) {
  if (questions.size() == 0) {
    throw runtime_error("driver.h: issue random");
  }
  Question storeQues = randomElement(questions);
  questions.erase(storeQues);
  return storeQues;
}
// ms 2
/* This function maps through all the pairs from answers and computes 
   the scores based off of the weight of the user input answer. 
   Weighs the pair information and stores it into the map and returns it.*/
map<char, int> scoresFrom(map<Question, int>& answers) {
  // #5: +O -> +2, +N-> +2, #4: +O -> +1, -E-> -1, #3: +E -> -1, -A-> +1,
  // #1: -O -> +2, -N-> +2,
  map<char, int> scores;
  for (pair<Question, int> x : answers) {
    for (pair<char, int> y : x.first.factors) {
      scores[y.first] += ((y.second) * (x.second - 3));
    }
  }
  return scores;
}

// ms3
/* O: 2 C:3 E:5
 (2)^2 + (3)^2 + (5)^2
 sqrt( 2^2 ) + sqrt( 3^2 ) + sqrt(5^2)
 This function normalizes the scores based off of the user input. 
 It first squares the number and stores it into the value of sum. 
 If sum is not empty than it squares sum and stores it into a value,
 it then divides each element in scores by the value and returns the result. 
 If the scores are empty a runtime error is thrown
 */
map<char, double> normalize(map<char, int>& scores) {
  // check to make sure scores isnt 0
  if (scores.size() == 0) {
    throw runtime_error("driver.h: issue normalize");
  }
  // add values into a sum var
  double sum = 0;
  map<char, double> result;
  for (pair<char, int> x : scores) {
    sum += pow(x.second, 2);
  }
  // check sum then divide and store into result
  if (sum != 0) {
    double value = sqrt(sum);
    for (pair<char, int> y : scores) {
      result[y.first] = (y.second / value);
    }
  }
  else
    throw runtime_error("driver.h: issue normalize");

  return result;
}

// ms4
/* This function takes two sets of the normalized values as inputs based off the corresponding scores
  & multiplies & adds them together to calculate the similarity of both of the normalized scores. 
  By using the cosine similarity range of -1 to 1, 
  wereable to return the similarity by the formula.
*/
double cosineSimilarityOf(const map<char, double>& lhs,
                          const map<char, double>& rhs) {
  double similarity = 0;
  // make sure letters are matching
  // if statement to compare
  // lhs.count('O') if returns 1 - exists
  double rhsval = 0;
  for (pair<char, double> x : lhs) {
      if (rhs.count(x.first) == 1) {
        rhsval = rhs.at(x.first);  // constant
        similarity += (rhsval * x.second);
        }
  }
    return similarity;
}

// ms 5
/* This function takes the raw OCEAN scores as user input and a set of fictional people,
  it then returns the returns the person with the highest cosine similarity. If the 
  people set is empty it will throw a runtime error.
*/
Person mostSimilarTo(map<char, int>& scores, set<Person>& people) {
  if (people.size() == 0) {
    throw runtime_error("driver.h: similar to");
  }
  Person person;
  double scoresUpdated;
  double maximum = -1;
  map<char, double> normalizedScores = normalize(scores);
  map<char, double> currentPerson;
  for (Person newPerson : people) {
    currentPerson = normalize(newPerson.scores);
    scoresUpdated = cosineSimilarityOf(normalizedScores, currentPerson);
    if (scoresUpdated > maximum) {
      maximum = scoresUpdated;
      person = newPerson;
    }
  }
  return person;
}
