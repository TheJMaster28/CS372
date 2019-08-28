// Jeffrey Lansford
// Lab 1 CS372
// 08-28-2019
#include <iostream>
using namespace std;

// [[Rcpp::export]]
//  fibonacci alogrthim with a linear runtime
int fib2(int n) {
  // creates a dynamic array 
  int *history;
  history = new int[n+1];
  history[0] = 0;
  history[1] = 1;
  // gets 2 previous results from histroy array and adds to new value
  for (int i = 2; i < n+1; i++) {
    history[i] = history[i-1] + history[i-2];
  }
  // get last value of sqeuace 
  int returnValue = history[n];
  // delete array for garbage removal
  delete [] history;
  return returnValue;
  
}

// tests fibonacci sequace function 
int fib2test() {
  bool passed = true;
  int cases[] = {0,1,2,3,4,5,6,7,8,9};
  int tests[] = {0,1,1,2,3,5,8,13,21,34};
  // checks if output is correct 
  for ( int i=0; i <10; i++) {
    if ( fib2(cases[i]) != tests[i] ) {
      passed = false;
      break;
    } 
  }
  
  if ( passed ) {
    cout<<"fib2 has passed"<<endl;
  }
  else {
    cout<<"fib2 has failed"<<endl;
  }
  return 0;
}

int main () {
  fib2test();
  return 0;
}

//----------------------------------------------------

/*** R 
k <- 10
ns <- (1:k) * 100000000
runtime <- vector(length=k)

for ( i in 1:k) {
  n <- ns[i]
  runtime[i] <- system.time(fib2(n))["user.self"]
}
plot(ns, runtime, type="b", xlab="n", ylab="runtime (second)")
grid(col="blue")

*/
