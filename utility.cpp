/*********************************
 *  ___      __      ___        * 
 *  \  \    /  \    /  /       *
 *   \  \  / __ \  /  /  **     *
 *    \  \/ /  \ \/  /  *  *     *
 *     \   /    \   /  * ** *   *
 *      ---      ---    SIAO     *
 * ******************************/
#ifndef _UTILITY_CPP_
#define _UTILITY_CPP_

#include "utility.hpp"

#include <algorithm>
#include <fstream>
#include <cmath>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#define PI 3.1415926535898
#define kB 0.01

using namespace std;

double arctan(const int &dx, const int &dy) {
    if (dx == 0) {
        if (dy == 0) return 0;
        else return (dy > 0)? 90:270;
    } else {
        double angle = atan((double)dy / (double)dx) * 180 / PI;
        if (dx > 0) return (angle >= 0)? angle:(360 + angle);
        else return (angle >= 0)? (180 + angle):(180 - angle);
    }
}

// diffE is a positive number
double BoltzmannProb(const double &diffE, const double &temperature) {
    return exp(- diffE / (kB * temperature));
}

// extract the first number in a string
int extract_int(const string &term) {
    size_t start_p = term.find_first_of("-0123456789"); 
    int i;
    istringstream(term.substr(start_p, term.size() - start_p)) >> i;
    return i;
}

// extract all numbers in a string
vector<int> extract_ints(const string &term) {
	// split string term into string vector by spaces
    istringstream ss(term);
	string token;
	vector<int> data;
	while(getline(ss, token, ' ')) data.push_back(extract_int(token));
    return data;
}

// generate a random number between [lower, upper)
// or generate a random probability
double generateRandom(int lower, int upper) {
    random_device rd;
    if (upper <= 0 || lower >= upper) return rd() % 10000 / (double)10000;
    else return rd() % (upper - lower) + lower;
}

// read CVRP data file and parse to a vector
// first vector contains dimension and capacity
// next 'dimension' vectors contain locations (x, y)
// then the customer demands (demand)
vector< vector<int> > readFile(const char *path) {
    ifstream file(path);
    string line;
    vector< vector<int> > data;

    vector<int> constraint;
    // read the first line and second line to get the dimension and capacity
    getline(file, line);
    // extract demand
    constraint.push_back(extract_int(line));
    getline(file, line);
    // extract capacity
    constraint.push_back(extract_int(line));
    data.push_back(constraint);
    
    // ignore the node section start line
    file.ignore(256, '\n');
   
    // extract locations
    for (int i = 0; i < constraint[0]; ++i) {
        getline(file, line);
        vector<int> p = extract_ints(line);
        // p[1]: x-coordinate, p[2]: y-coordinate
        // -> p[0]: x, p[1]: y
        p.erase(p.begin());
        data.push_back(p);
    }
    
    //ignore the demand section start line	
	file.ignore(256, '\n');

    vector<int> demand;
	// read the customer demand to the 1D array
	for (int j = 0; j < constraint[0]; ++j) {
		getline(file, line);
		demand.push_back(extract_ints(line)[1]);
    }
    data.push_back(demand);
   
    return data;
}

#endif
