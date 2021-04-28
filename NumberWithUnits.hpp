#pragma once

#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <stdexcept>

const float TOLERANCE = 0.001;

namespace ariel {
    class NumberWithUnits {
        double amount;
        std::string unit;
        static std::map<std::string, std::map<std::string, double>> unitConverter;

    public:
        NumberWithUnits(double amount, std::string unit) {
            if (unitConverter.count(unit) != 0) {
                this->unit = std::move(unit);
                this->amount = amount;
            } else {
                throw std::invalid_argument{"Unit doesn't exists!"};
            }
        }

        static void read_units(std::ifstream &file); // Reads a given conversion units file and converts these units accordingly.

        //----------------------------------------
        // Binary operators - + , += , - , -=
        //----------------------------------------

        NumberWithUnits operator+(const NumberWithUnits &other);

        NumberWithUnits &operator+=(const NumberWithUnits &other);

        NumberWithUnits operator-(const NumberWithUnits &other);

        NumberWithUnits &operator-=(const NumberWithUnits &other);

        //----------------------------------------
        // Unary operators - + , -
        //----------------------------------------

        NumberWithUnits operator+() const;

        NumberWithUnits operator-() const;

        //----------------------------------------------------
        // Comparison operators - > , >= , < , <= , == , !=
        //----------------------------------------------------

        bool operator>(const NumberWithUnits &other) const;

        bool operator>=(const NumberWithUnits &other) const;

        bool operator<(const NumberWithUnits &other) const;

        bool operator<=(const NumberWithUnits &other) const;

        bool operator==(const NumberWithUnits &other) const;

        bool operator!=(const NumberWithUnits &other) const;

        //-----------------------------------------
        // Prefix increment / decrement operators
        //-----------------------------------------

        NumberWithUnits &operator++();

        NumberWithUnits &operator--();

        //------------------------------------------
        // Postfix increment / decrement operators
        //------------------------------------------

        NumberWithUnits operator++(int);

        NumberWithUnits operator--(int);

        //----------------------------------
        // Multiply operators
        //----------------------------------

        NumberWithUnits operator*(double num);

        friend NumberWithUnits operator*(double num, NumberWithUnits const &other);

        //----------------------------------
        // friend global IO operators
        //----------------------------------

        friend std::ostream &operator<<(std::ostream &os, const NumberWithUnits &other);

        friend std::istream &operator>>(std::istream &is, NumberWithUnits &other);

        static double conversionBetweenTwoUnits(std::string const &thisUnit, std::string const &otherUnit, double otherAmount);

        static bool unitIsValid(std::string const &unit);
    };
}
