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
#include <math.h>
using namespace std;

template <typename T>
void printVector( vector<T>  &a) {
    for ( int i = 0; i < a.size(); i++) {
        cout<<a[i]<<' ';
    }
    cout<<endl;
}

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
  vector<T> r;
  
  T max;

  for ( int i = 0; i < m.nrow(); i++ ) {
    max = m(i,0);
    for ( int j = 0; j < m.ncol(); j++ ) {
      if ( max < m(i, j) )
        max = m(i, j);
    }
    r.push_back(max);
  }

  return r;
}



// Do this First!
// Assume m is a monotonic Matrix
template<typename T>
vector<T> find_row_maxima(const matrix<T> & m) 
{
  // Your code here:
  // divide-and-conquer on monotonic matrix

  vector<T> r;
  
  if ( m.nrow() == 1 ) {
    T max = m(0,0);
    for ( int i = 0 ; i < m.ncol(); i++ ) {
      T num = m(i,0);
      if ( max <  num) {
        max=num;
        
      }
    }
    r.push_back(max);
    return r;
  }
  
  if ( m.nrow() == 2 ) {

    double *firstHalfA;
    firstHalfA = new double[( m.nrow() / 2) * m.ncol() ];
    int pointerForElement = 0;
    for ( size_t i = 0; i < m.ncol(); i++) {
      for ( size_t j = 0; j < 1; j++){
        firstHalfA[pointerForElement] = m(j, i);
        pointerForElement++;
      }
    }

    vector<double> firstHalfM (firstHalfA, firstHalfA+(1 * m.ncol() ));

    matrix<T> firstHalf(firstHalfM, 1, m.ncol() );


    double *secondHalfA;
    secondHalfA = new double[1 * m.ncol() ];
    pointerForElement = 0;

    for ( size_t i = 0; i < m.ncol(); i++) {
      for ( size_t j = 1 ; j < m.nrow(); j++){
        secondHalfA[pointerForElement] = m(j, i);
        pointerForElement++;
      }
    }

    vector<double> secondHalfM (secondHalfA, secondHalfA+( 1 * m.ncol() ));

    matrix<T> secondHalf(secondHalfM, 1, m.ncol() );


    vector<T> r1 = find_row_maxima(firstHalf);
    vector<T> r2 = find_row_maxima(secondHalf);
    r1.insert(r1.end(), r2.begin(), r2.end());
    
    
    r = r1;
    return r;
    
  }

  else {
    // find middle row for either even or odd number of rows

    size_t middleRow = ceil(m.nrow()/2);
    T max = m(0,0);
    for ( size_t i = 0 ; i < m.ncol(); i++ ) {
      if ( max < m(i,middleRow) ) {
        max=m(i,middleRow);
      }
    }

    
    // SPilt Matix in half
    double *firstHalfA;
    firstHalfA = new double[( m.nrow() / 2) * m.ncol() ];
    int pointerForElement = 0;
    for ( size_t i = 0; i < m.ncol(); i++) {
      for ( size_t j = 0; j < middleRow; j++){
        firstHalfA[pointerForElement] = m(j, i);
        pointerForElement++;
      }
    }

    vector<double> firstHalfM (firstHalfA, firstHalfA+(( m.nrow() / 2) * m.ncol() ));

    matrix<T> firstHalf(firstHalfM, floor(m.nrow() / 2), m.ncol() );
    

    double *secondHalfA;
    secondHalfA = new double[( m.nrow() / 2) * m.ncol() ];
    pointerForElement = 0;

    for ( size_t i = 0; i < m.ncol(); i++) {
      for ( size_t j = middleRow + 1 ; j < m.nrow(); j++){
        secondHalfA[pointerForElement] = m(j, i);
        pointerForElement++;
      }
    }

    size_t size= 1;
    if ( m.nrow() % 2 == 0 ) {
      size = (m.nrow() / 2 ) -1;
    }
    else {
      size = m.nrow() / 2;
    }
    vector<double> secondHalfM (secondHalfA, secondHalfA+(  size * m.ncol() ));

    matrix<T> secondHalf(secondHalfM, size, m.ncol());
    
    vector<T> r1 = find_row_maxima(firstHalf);
    vector<T> r2 = find_row_maxima(secondHalf);
    r1.insert(r1.end(),max);
    r1.insert(r1.end(), r2.begin(), r2.end());
  
  
    r = r1;
    return r;

  }
  
  
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
  vector<double> answer = find_row_maxima(mat);
  return answer;
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
