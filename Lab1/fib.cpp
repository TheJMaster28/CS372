#include <iostream>
using namespace std;
// [[Rcpp::export]]
int fib1(int n) {
	if ( n == 0 ) return 0;
	if ( n == 1 ) return 1;
	return fib1(n-1) + fib1(n-2);
}
int fib1test() {
  bool passed = true;
  int cases[] = {0,1,2,3,4,5,6};
  int tests[] = {0,1,1,2,3,5,8};
  for ( int i=0; i <7; i++) {
    if ( fib1(cases[i]) != tests[i] ) {
      passed = false;
      break;
    } 
  }
  
  if ( passed ) {
    cout<<"fib1 has passed"<<endl;
  }
  else {
    cout<<"fib1 has failed"<<endl;
  }
}

int main () {
        fib1test();
        return 0;
}

//----------------------------------------------------

/*** R 
k <- 10
ns <- 34+(1:k)
runtime <- vector(length=k)

for ( i in 1:k) {
  n <- ns[i]
  runtime[i] <- system.time(fib1(n))["user.self"]
}
plot(ns, runtime, type="b", xlab="n", ylab="runtime (second)")
grid(col="blue")

*/
