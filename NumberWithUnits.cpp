#include "NumberWithUnits.hpp"
#include <string>
#include <sstream>
#include <iostream>

using namespace ariel;
using namespace std;

map<string, std::map<string, double>> NumberWithUnits::unitConverter;

void NumberWithUnits::read_units(ifstream &file) {
    string line;
    string unit1, equals, unit2;
    double num1 = 0, num2 = 0;
    while (getline(file, line)) {

        istringstream ss(line);
        ss >> num1 >> unit1 >> equals >> num2 >> unit2;

        unitConverter[unit1][unit1] = num1;
        unitConverter[unit2][unit2] = num1;
        unitConverter[unit1][unit2] = num2;
        unitConverter[unit2][unit1] = num1 / num2;

        for (auto const &unitA: NumberWithUnits::unitConverter[unit1]) {
            if ((unitA.first != unit1 && unitA.first != unit2) || unitConverter[unitA.first].count(unit2) == 0) {
                double temp1 = NumberWithUnits::unitConverter[unitA.first][unit1];
                double temp2 = num2 * temp1;
                unitConverter[unitA.first][unit2] = temp2;
                unitConverter[unit2][unitA.first] = num1 / temp2;
            }
            for(auto const& unitB : unitConverter[unit2]){
                if(unitConverter[unitA.first].count(unitB.first) == 0) {
                    double temp1 = unitConverter[unit2][unitB.first];
                    double temp2 = unitConverter[unitA.first][unit2];
                    unitConverter[unitA.first][unitB.first] = temp1 * temp2;
                    unitConverter[unitB.first][unitA.first] = num1 / (temp1 * temp2);
                }
            }
        }
    }
    file.close();
}

//----------------------------------------
// Binary operators - + , += , - , -=
//----------------------------------------

NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits &other) {
    double result = conversionBetweenTwoUnits(this->unit, other.unit, other.amount);
    return NumberWithUnits(this->amount + result, this->unit);
}

NumberWithUnits &NumberWithUnits::operator+=(const NumberWithUnits &other) {
    double result = conversionBetweenTwoUnits(this->unit, other.unit, other.amount);
    this->amount += result;
    return *this;
}

NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits &other) {
    double result = conversionBetweenTwoUnits(this->unit, other.unit, other.amount);
    return NumberWithUnits(this->amount - result, this->unit);
}

NumberWithUnits &NumberWithUnits::operator-=(const NumberWithUnits &other) {
    double result = conversionBetweenTwoUnits(this->unit, other.unit, other.amount);
    this->amount -= result;
    return *this;
}

//----------------------------------------
// Unary operators - + , -
//----------------------------------------

NumberWithUnits NumberWithUnits::operator+() const {
    return NumberWithUnits(1 * this->amount, this->unit);
}

NumberWithUnits NumberWithUnits::operator-() const {
    return NumberWithUnits(-1 * this->amount, this->unit);
}

//----------------------------------------------------
// Comparison operators - > , >= , < , <= , == , !=
//----------------------------------------------------

bool NumberWithUnits::operator>(const NumberWithUnits &other) const {
    double result = conversionBetweenTwoUnits(this->unit, other.unit, other.amount);
    return this->amount > result;
}

bool NumberWithUnits::operator>=(const NumberWithUnits &other) const {
    double result = conversionBetweenTwoUnits(this->unit, other.unit, other.amount);
    return abs(this->amount - result) <= TOLERANCE || this->amount > result;
}

bool NumberWithUnits::operator<(const NumberWithUnits &other) const {
    double result = conversionBetweenTwoUnits(this->unit, other.unit, other.amount);
    return this->amount < result;
}

bool NumberWithUnits::operator<=(const NumberWithUnits &other) const {
    double result = conversionBetweenTwoUnits(this->unit, other.unit, other.amount);
    return abs(this->amount - result) <= TOLERANCE || this->amount < result;
}

bool NumberWithUnits::operator==(const NumberWithUnits &other) const {
    double result = conversionBetweenTwoUnits(this->unit, other.unit, other.amount);
    return abs(this->amount - result) <= TOLERANCE;
}

bool NumberWithUnits::operator!=(const NumberWithUnits &other) const {
    double result = conversionBetweenTwoUnits(this->unit, other.unit, other.amount);
    return !(this->amount == result);
}

double NumberWithUnits::conversionBetweenTwoUnits(const string &thisUnit, const string &otherUnit, double otherAmount) {
    if (unitConverter.count(otherUnit) != 0 && unitConverter[otherUnit].count(thisUnit) != 0) {
        return unitConverter[otherUnit][thisUnit] * otherAmount;
    }
    throw invalid_argument{"Units do not match - [" + otherUnit + "] cannot be converted to [" + thisUnit + "]"};
}

//-----------------------------------------
// Prefix increment / decrement operators
//-----------------------------------------

NumberWithUnits &NumberWithUnits::operator++() {
    this->amount++;
    return *this;
}

NumberWithUnits &NumberWithUnits::operator--() {
    this->amount--;
    return *this;
}

//------------------------------------------
// Postfix increment / decrement operators
//------------------------------------------

NumberWithUnits NumberWithUnits::operator++(int) {
    NumberWithUnits copy = *this;
    this->amount++;
    return copy;
}

NumberWithUnits NumberWithUnits::operator--(int) {
    NumberWithUnits copy = *this;
    this->amount--;
    return copy;
}

//----------------------------------
// Multiply operators
//----------------------------------

NumberWithUnits NumberWithUnits::operator*(double num) {
    return NumberWithUnits(this->amount * num, this->unit);
}

NumberWithUnits ariel::operator*(double num, NumberWithUnits const &other) {
    return NumberWithUnits(other.amount * num, other.unit);
}

//----------------------------------
// friend global IO operators
//----------------------------------

ostream &ariel::operator<<(ostream &os, const NumberWithUnits &other) {
    return os << other.amount << "[" << other.unit << "]";
}

istream &ariel::operator>>(istream &is, NumberWithUnits &other) {
    double amount = 0;
    char firstBracket = ' ', lastBracket = ' ';
    string unit;

    is >> amount >> firstBracket >> unit;
    other.amount = amount;
    if (unit.back() == ']' && unit.length() >= 2) {
        unit = unit.substr(0, unit.size() - 1);
        if (NumberWithUnits::unitIsValid(unit)) {
            other.unit = unit;
        }
    } else {
        is >> lastBracket;
        if (lastBracket != ']') {
            throw invalid_argument{"invalid input!"};
        }
        if (NumberWithUnits::unitIsValid(unit)) {
            other.unit = unit;
        }
    }
    return is;
}

bool NumberWithUnits::unitIsValid(string const &unit) {
    if (unitConverter.count(unit) != 0) {
        return true;
    }
    throw invalid_argument{"Unit doesn't exists!"};
}





