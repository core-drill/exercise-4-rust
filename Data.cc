#include "Data.hh"

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <cmath>

using namespace std;

Data::Data(const std::string& filename) {
  ifstream file(filename);

  // check if file opened correctly
  if (!file.is_open()) {
    throw std::runtime_error("Error could not open file with name: " +
                             filename);
  }

  int size = -1;
  file >> size;

  // read in data from file: m_siz+1 bin edges
  for (int i = 0; i < size + 1; ++i) {
    double edge;
    file >> edge;
    m_bins.push_back(edge);
  }
  // read in data from file: m_siz bin contents
  for (int i = 0; i < size; ++i) {
    double entries;
    file >> entries;
    m_data.push_back(entries);
  }

  for (int i = 0; i < size; ++i) {
    double error;
    file >> error;
    m_errors.push_back(error);
  }

  // done! close the file
  file.close();

  assertSizes();
};


Data::Data(const std::string& file_A, const std::string& file_B) {
    Data data_A(file_A);
    Data data_B(file_B);

    if (data_A.size() != data_B.size()) {
        throw std::runtime_error("Error: Data sets have different sizes.");
    }

    int n_compatible = 0;
    int n = 3;
    n_compatible = data_A.checkCompatibility(data_B, n);
    printf("Number of compatible bins between %s and %s with %d sigma: %d\n",
           file_A.c_str(), file_B.c_str(), n, n_compatible);

    m_bins = data_A.m_bins;
    m_data.resize(data_A.size());
    m_errors.resize(data_A.size());

    for (unsigned int i = 0; i < data_A.size(); ++i) {
        double a = data_A.measurement(i);
        double b = data_B.measurement(i);
        double w_a = 1.0 / (data_A.error(i) * data_A.error(i));
        double w_b = 1.0 / (data_B.error(i) * data_B.error(i));
        m_data[i] = (w_a * a + w_b * b) / (w_a + w_b);
        m_errors[i] = std::sqrt(1.0 / (w_a + w_b));
    }

    assertSizes();
}

Data::Data(const std::string& file_A, const std::string& file_B, const std::string& file_C, const std::string& file_D) {
    Data data_A(file_A);
    Data data_B(file_B);
    Data data_C(file_C);
    Data data_D(file_D);
    if (data_A.size() != data_B.size() || data_A.size() != data_C.size() || data_A.size() != data_D.size()) {
        throw std::runtime_error("Error: Data sets have different sizes.");
    }
    m_bins = data_A.m_bins;
    m_data.resize(data_A.size());
    m_errors.resize(data_A.size());
    for (unsigned int i = 0; i < data_A.size(); ++i) {
        double a = data_A.measurement(i);
        double b = data_B.measurement(i);
        double c = data_C.measurement(i);
        double d = data_D.measurement(i);
        double w_a = 1.0 / (data_A.error(i) * data_A.error(i));
        double w_b = 1.0 / (data_B.error(i) * data_B.error(i));
        double w_c = 1.0 / (data_C.error(i) * data_C.error(i));
        double w_d = 1.0 / (data_D.error(i) * data_D.error(i));
        m_data[i] = (w_a * a + w_b * b + w_c * c + w_d * d) / (w_a + w_b + w_c + w_d);
        m_errors[i] = std::sqrt(1.0 / (w_a + w_b + w_c + w_d));
    }
}


void Data::assertSizes() { assert(m_data.size() + 1 == m_bins.size()); }

int Data::checkCompatibility(const Data & in, int n) {
    int n_compatible = 0;
    for (unsigned int i = 0; i < size(); i++) {
        double diff = measurement(i) - in.measurement(i);
        double combined_sigma = std::sqrt(error(i)*error(i) + in.error(i)*in.error(i));
        if (std::abs(diff) < n * combined_sigma) {
            n_compatible += 1;
        }
    }
     return n_compatible;
}

double background_function(double x) {
    double alpha = 0.005;
    double beta = -0.00001;
    double gamma = 0.08;
    double delta = 0.015;

    return alpha + beta * x + gamma * std::exp(-delta * x);
  } 

double Data::chiSquare_ndof() {
    double chi2 = 0.0;
    int ndof = size();
    for (unsigned int i = 0; i < size(); i++) {
        double x = 0.5 * (binLow(i) + binHigh(i));
        double expected = background_function(x);
        double observed = measurement(i);
        double err = error(i);
        chi2 += ((observed - expected) * (observed - expected)) / (err * err);
    }
    return chi2 / ndof;
}

