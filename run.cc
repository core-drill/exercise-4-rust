#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <cmath>
#include <vector>

#include "Data.hh"

// generic function comparing two values of some type T used later for int and
// double
template <class T>
bool testEqual(const std::string& name, T expected, T real) {
  if (expected != real) {
    std::cout << "(" << name << ": <" << expected << ">"
              << ", " << real << ") ";
    return false;
  }
  return true;
}

bool testReadingSize() {
  std::cout << "testReadingSize...";
  Data datA("testA");
  return testEqual<int>("size", 1, datA.size());
}

bool testReadingMeasurement() {
  std::cout << "testReadingMeasurement...";
  Data datA("testA");
  return testEqual("measurement", 10., datA.measurement(0));
}

bool testReadingBinEdges() {
  std::cout << "testReadingBinEdges...";
  Data datA("testA");
  return testEqual("bin low", 0., datA.binLow(0)) &&
         testEqual("bin high", 1., datA.binHigh(0));
}

bool testReadingErrors() {
  std::cout << "testReadingErrors...";
  Data datA("testA");
  return testEqual("error", 2., datA.error(0));
}

bool testCopyConstructor() {
  std::cout << "testCopyConstructor...";
  Data a("testA");
  Data b(a);
  Data c = a;  // equivalent to Data c(a)
  return testEqual("measurement", 10., b.measurement(0)) &&
         testEqual("measurement", 10., c.measurement(0));
}

int testcompatible(double data1, double data2 ,double sigma1, double sigma2) {
     double diff = data1 - data2;
     double combined_sigma = std::sqrt(sigma1*sigma1 + sigma2*sigma2);
     int n = 1;
     for(int i = 0; i < 5; i += 1) {
          if (std::abs(diff) <= n * combined_sigma) {
               break;
          }
          n += 1;
     }
     return n;
}

void runTests() {
  std::cout << "running tests...\n";
  std::vector<std::function<bool()> > tests(
      {testReadingSize, testReadingMeasurement, testReadingBinEdges,
       testReadingErrors, testCopyConstructor});
  for (auto test : tests)
    std::cout << (test() ? " ok" : " FAILED!") << std::endl;
}

int main() {
     using namespace std;

     cout << "******************************************************" << endl;
     runTests();
     cout << "******************************************************" << endl;
     // create an object which holds data of experiment A
     Data datA("exp_A");

     // here is the data from experiment A
     cout << "bin 27: from " << datA.binLow(27) << " to " << datA.binHigh(27)
          << endl;
     cout << "measurement of experiment A in bin 27: " << datA.measurement(27)
          << endl;
     cout << "error of experiment A in bin 27: " << datA.error(27) << endl;

     Data datB("exp_B");
     cout << "bin 27: from " << datB.binLow(27) << " to " << datB.binHigh(27)
          << endl;
     cout << "measurement of experiment B in bin 27: " << datB.measurement(27)
          << endl; 
     cout << "error of experiment B in bin 27: " << datB.error(27) << endl; 

     Data datC("exp_C");
     cout << "bin 27: from " << datC.binLow(27) << " to " << datC.binHigh(27)
          << endl;
     cout << "measurement of experiment C in bin 27: " << datC.measurement(27)
          << endl;
     cout << "error of experiment C in bin 27: " << datC.error(27) << endl;

     Data datD("exp_D");
     cout << "bin 27: from " << datD.binLow(27) << " to " << datD.binHigh(27)
          << endl;
     cout << "measurement of experiment D in bin 27: " << datD.measurement(27)
          << endl;
     cout << "error of experiment D in bin 27: " << datD.error(27) << endl;

     cout << "compatibility of A and B in bin 27: " 
          << testcompatible(datA.measurement(27), datB.measurement(27),
                         datA.error(27), datB.error(27)) << " sigma" << endl;
     
     cout << "compatibility of A and C in bin 27: " 
          << testcompatible(datA.measurement(27), datC.measurement(27),
                         datA.error(27), datC.error(27)) << " sigma" << endl;
     
     cout << "compatibility of A and D in bin 27: " 
          << testcompatible(datA.measurement(27), datD.measurement(27),
                         datA.error(27), datD.error(27)) << " sigma" << endl;
     
     cout << "compatibility of B and C in bin 27: " 
          << testcompatible(datB.measurement(27), datC.measurement(27),
                         datB.error(27), datC.error(27)) << " sigma" << endl;

     cout << "compatibility of B and D in bin 27: " 
          << testcompatible(datB.measurement(27), datD.measurement(27),
                         datB.error(27), datD.error(27)) << " sigma" << endl;
     
     cout << "compatibility of C and D in bin 27: " 
          << testcompatible(datC.measurement(27), datD.measurement(27),
                         datC.error(27), datD.error(27)) << " sigma" << endl;

     cout << "number of compatible bins between A and B with 3 sigma: " 
          << datA.checkCompatibility(datB, 3) << endl;
     
     cout << "number of compatible bins between A and C with 3 sigma: " 
          << datA.checkCompatibility(datC, 3) << endl;

     cout << "number of compatible bins between A and D with 3 sigma: "
          << datA.checkCompatibility(datD, 3) << endl;
     
     cout << "number of compatible bins between B and C with 3 sigma: " 
          << datB.checkCompatibility(datC, 3) << endl;
     
     cout << "number of compatible bins between B and D with 3 sigma: "
          << datB.checkCompatibility(datD, 3) << endl;
     
     cout << "number of compatible bins between C and D with 3 sigma: "
          << datC.checkCompatibility(datD, 3) << endl;

     Data sum12 = Data("exp_A", "exp_B");
     cout << "combined measurement of A and B (sum12)in bin 27: " 
          << sum12.measurement(27) << endl;

     cout << "chi2/ndof of experiment A: " << datA.chiSquare_ndof() << endl;
     cout << "chi2/ndof of experiment B: " << datB.chiSquare_ndof() << endl;
     cout << "chi2/ndof of experiment C: " << datC.chiSquare_ndof() << endl;
     cout << "chi2/ndof of experiment D: " << datD.chiSquare_ndof() << endl;

     Data sum_all = Data("exp_A", "exp_B", "exp_C", "exp_D");
     cout << "chi2/ndof of combined experiment (A,B,C,D): " 
          << sum_all.chiSquare_ndof() << endl;

  return 0;
}
