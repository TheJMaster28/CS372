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
  
  // base case if number of rows is 1
  if ( m.nrow() == 1 ) {

    // find maximum of the row
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
  
  // get middle row of the matrix
  size_t middleRow = ceil(m.nrow()/2.0);

  // do something special for 2x matrixs 
  if ( m.nrow() == 2 ) middleRow = 2;
  
  // exclude middle row for divison
  middleRow--;

  // find maximum of the middle row
  size_t maxColumn = 0;
  T max = m(middleRow,0);
  for ( size_t i = 0 ; i < m.ncol(); i++ ) {
    T value = m(middleRow,i);
    if ( max <  value ) {
      max=value;
      maxColumn = i;
    }

  // create first (top) half of matrix
  vector<T> firstHalfA;
  for ( size_t i=0; i <= maxColumn; i++ ) {
    for( size_t j=0; j < middleRow; j++ ) {
      firstHalfA.push_back(m(j,i));
    }
  }

  // create second (bottom) half of matrix
  vector<T> secondHalfA;
  for ( size_t i=maxColumn; i < m.ncol(); i++ ) {
    for( size_t j=middleRow+1; j < m.nrow(); j++ ) {
      secondHalfA.push_back(m(j,i));
    }
  }

  // create the matrixs
  matrix<T> firstHalf( firstHalfA, middleRow, maxColumn+1 );
  matrix<T> secondHalf( secondHalfA, m.nrow() - (middleRow+1), ( m.ncol() - maxColumn ) );
    
  // find first half maxiumias  
  vector<T> r1 = find_row_maxima(firstHalf);
  
  // insert middle maximum 
  r.insert(r.end(), r1.begin(), r1.end());
  r.push_back(max);

  // if not a 2x matrix, insert bottom half  
  if ( m.nrow() != 2 ) {
    vector<T> r2 = find_row_maxima(secondHalf);
    r.insert(r.end(), r2.begin(), r2.end());
  }
    
  return r;

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

  double y[] = { 
                  1, -1, -2.5, -3, -1, -1.2, -3, -10, -54, -90, -100.34, -1, 0, -3, 
                  2,  6,   1,   4,  5,   1,  -1,  -2,  3,  4,         1,  5, 2,  0,
                  0,   1,   9,  7,  6,   2,   1,   2,  4,  6,         8, 10, 3,  9
  };

  vector<double> v1(y,y+42);

  vector<double> truth = {2,6,9,7,6,2,1,2,4,6,8,10,3,9};

  if (rmfun(v1,14, 3) != truth ) {
    cout << "ERROR: failed test 2!" << endl;
    passed = false;
  }


  double z[] = {
                  4,1,1,1,1,
                  1,4,1,1,1,
                  1,1,4,1,1,
                  1,1,1,4,1,
                  1,1,1,1,4,
                  1,1,1,1,1
  };

  vector<double> v2(z,z+30);

  truth = {4,4,4,4,4};

  if (rmfun(v2,5,6) != truth ) {
    cout << "ERROR: failed test 3!" << endl;
    passed = false;
  }

  double a[] = {
                  1,-1,-1,-1,
                  -1,-1,-1,-1,
                  -1,-1,-1,-1,
                  -1, 1, 1, 1,
  };

  vector<double> v3(a,a+16);

  truth = {1,1,1,1};

  if (rmfun(v3,4,4) != truth ) {
    cout << "ERROR: failed test 4!" << endl;
    passed = false;
  }


double b[] = {
                  5,1,1,
                  1,5,5,
  };

  vector<double> v4(b,b+6);

  truth = {5,5,5};

  if (rmfun(v4,3,2) != truth ) {
    cout << "ERROR: failed test 5!" << endl;
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
