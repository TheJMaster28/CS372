// MatrixSearch-Skeleton.cpp
// Joe Song
// Created: Sept 11, 2016
// Modified: 
//   Sept 4, 2019

//#include <Rcpp.h>
//using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]

#include <vector>
#include <iostream>
using namespace std;

template <typename T>
class matrix {
private:
  vector<T> m_vec;
  size_t m_nrow;
  size_t m_ncol;
  
public:
  matrix(const vector<T> &v, size_t nrow, size_t ncol):
    m_vec(v), m_nrow(nrow), m_ncol(ncol) {
  } 
  
  size_t nrow() const { return m_nrow; }
  size_t ncol() const { return m_ncol; }
  
  T & operator () (size_t i, size_t j) {
    return m_vec[i + j * m_nrow];
  }
  
  const T & operator () (const size_t i, const size_t j) const {
    return m_vec[i + j * m_nrow];
  }
  
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const matrix<T> & m)
{
  for(size_t i=0; i<m.nrow(); ++i) {
    for(size_t j=0; j<m.ncol(); ++j) {
      out << m(i, j) << ' ';
    }
    out << endl;
  }
  return out;
}

template<typename T>
vector<T> find_row_maxima_itr(const matrix<T> & m) 
{
  // Your code here:
  // Iteratively find the row maximum of row by row:
}



// Do this First!
// Assume m is a monotonic Matrix
template<typename T>
vector<T> find_row_maxima(const matrix<T> & m) 
{
  // Your code here:
  // divide-and-conquer on monotonic matrix

}

//[[Rcpp::export]]
void print_matrix(const vector<double> & v, size_t nrow, size_t ncol)
{
  matrix<double> mat(v, nrow, ncol);

  cout << "Input matrix is:" << endl;
  cout << mat << endl;
}

//[[Rcpp::export]]
vector<double> row_maxima_itr(const vector<double> & v, size_t nrow, size_t ncol)
{
  matrix<double> mat(v, nrow, ncol);
  return find_row_maxima_itr(mat);
}

//[[Rcpp::export]]
vector<double> row_maxima(const vector<double> & v, size_t nrow, size_t ncol)
{
  matrix<double> mat(v, nrow, ncol);
  return find_row_maxima(mat);
}

bool test_row_maxima(vector<double> (*rmfun) (const vector<double> & v, size_t nrow, size_t ncol)) 
{
  bool passed = true;
  /* Monotonic matrix example 1:
   0,  4, -1, 2.5, -4, 
  -3, 8, -10, 2, 7,
  -4, -3, -1, -100, -5.5,
  0, 2, 0.3, -3, 2.5,
  1, 0, 1, 2, 3,
  -8, 9, 2, 5, 10};
  */
  // x is column major vectorization of the matrix
  double x[] = {  0,  -3,   -4,   0, 1, -8,
                  4,   8,   -3,   2, 0,  9,
                  -1, -10,   -1, 0.3, 1,  2,
                  2.5,   2, -100,  -3, 2,  5,
                  -4,   7, -5.5, 2.5, 3, 10};
  
  vector<double> v(x, x+30);
  
  double rmax_truth[] = {4, 8, -1, 2.5, 3, 10};
  
  if(rmfun(v, 6, 5) != vector<double>(rmax_truth, rmax_truth+6)) {
    cout << "ERROR: failed test 1!" << endl;
    passed = false;
  }
  
  return passed;
}

//[[Rcpp::export]]
bool testall() 
{
  bool passed = true;
  if(!test_row_maxima(row_maxima_itr)) {
    cout << "ERROR: row_maxima_itr() failed some test!" << endl;
    passed = false;
  }

  if(!test_row_maxima(row_maxima)) {
    cout << "ERROR: row_maxima() failed some test!" << endl;
    passed = false;
  }
  
  if(passed) {
    cout << "All tests passed. Congratulations!" << endl;
  }
  return passed;
}

int main()
{
  testall();
  return 0;
}
  
/*** R

if(!testall()) stop()

random.monotone.matrix <- function(nrow, ncol) 
{
  m <- matrix(rnorm(nrow*ncol), nrow=nrow, ncol=ncol)
  row.maxima.indices <- apply(m, 1, which.max)
  o <- order(row.maxima.indices)
  m <- m[o, ]
}

# Your R code for run time evaluation and visualization
*/
