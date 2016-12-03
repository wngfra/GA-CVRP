#ifndef _UTILITY_HPP_
#define _UTILITY_HPP_

#include <string>
#include <vector>

using namespace std;

// calculate arctan to [0, 360] degress
double arctan(const int&, const int&);

// calculate the Boltzmann Probability
double BoltzmannProb(const double&, const double&);

// extract the first number in a string
int extract_int(const string&);

// extract all numbers in a string
vector<int> extract_ints(const string&);

// generate a random number 
double generateRandom(int=0, int=0);

// read CVRP data file and parse to a vector
vector< vector<int> > readFile(const char*);

// generic functions
template <typename T>
bool contain(const vector<T> &vec, const T &elem) {
    if (find(vec.begin(), vec.end(), elem) == vec.end()) return false;
    else return true;
}

#endif
