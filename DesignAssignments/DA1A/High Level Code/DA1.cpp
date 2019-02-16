/*
 * Steven Lee, 5003887117, Created
 * CPE 301 Design Assignment 1
 * Multiplication with 16-bit multiplicand and 8-bit multiplier
 * Result limits: 24-bits
 */

    #include<iostream>
    #include<cmath>
    using namespace std;

    int main() {

      unsigned int multiplicand;
      unsigned int multiplier;
      unsigned int result;

      cout << "Enter multiplicand: 0x";
      cin >> hex >> multiplicand;

      cout << "Enter multiplier: 0x";
      cin >> hex >> multiplier;

      result = multiplicand * multiplier;

      cout << "Result: 0x" << hex << result << endl;

      return 0;
    }
