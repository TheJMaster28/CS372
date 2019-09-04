// Jeffrey Lansford
// Lab2
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
using namespace std;
// [[Rcpp::plugins(cpp11)]]

void paddingVector( vector<bool> &x, vector<bool> &y ) {

    while ( x.size() != y.size() ) {
        if ( x.size() > y.size() ) {
            y.push_back(0);
        }
        else {
            x.push_back(0);
        }

    }

}

void printVector( vector<bool>  &a) {
    for ( int i = a.size()-1; i >= 0; i--) {
        cout<<a[i]<<' ';
    }
}

// adds two binary numbers in a boolean vector
vector<bool> addingBinary( vector<bool> &x, vector<bool> &y) {
    vector<bool> z;
    bool carry=0;

    vector<bool> xTemp = x;
    vector<bool> yTemp = y;

    paddingVector(xTemp, yTemp);

    for ( size_t k = 0; ( k < xTemp.size() || k < yTemp.size() ); k++) {
        z.push_back(((xTemp[k] ^ yTemp[k] ^ carry )));
        carry = (( xTemp[k] & yTemp[k] ) | (xTemp[k] & carry)) | (yTemp[k] & carry);
    }

    if ( carry ) {
        z.push_back(1);
    }
    
    return z;

}

vector<bool> subtractingBinary( vector<bool> &x, vector<bool> &y) {

    vector<bool> xTemp = x;
    vector<bool> yTemp = y;

    paddingVector(xTemp, yTemp);
    
    vector<bool> yComplement;
    
    yComplement.assign(yTemp.size(), 0);
    
    for( size_t i = 0; i < yTemp.size(); i++) {
        yComplement[i] = (yTemp[i] != 1);
    }

    vector<bool> one = {1};
    
    while ( one.size() != xTemp.size() ) {
        one.push_back(0);
    }
    
    vector<bool> z;
    bool carry=0;

    for ( size_t k = 0; ( k < yComplement.size() || k < one.size()); k++) {
        z.push_back(((yComplement[k] ^ one[k] ^ carry )));
        carry = (( yComplement[k] & one[k] ) | (yComplement[k] & carry)) | (one[k] & carry);
    }

    carry=0;

    vector<bool> result;

    paddingVector(xTemp, z);

    for ( size_t     k = 0; ( k < z.size() || k < xTemp.size()); k++) {
        result.push_back(((z[k] ^ xTemp[k] ^ carry )));
        carry = (( z[k] & xTemp[k] ) | (z[k] & carry)) | (xTemp[k] & carry);
    }

    return result;
}

// multiples to boolean vectors itleriave
// [[Rcpp::export]]
vector<bool> multiply_itr(vector<bool> & x, vector<bool> & y ,bool debug) {
    vector<bool> z;
    vector<bool> inter;
    
    // fills vector with zeros to inlize vector
    z.assign(x.size() + y.size(), 0);
    
    // goes through each bit from y byte
    for ( size_t i = 0; i < y.size(); i++ ) {
        
        // makes intermedite value zero
        inter.assign(x.size(), 0);
        
        // if y's bit is one, then copy x to intermedite
        if ( y[i] ) {
            inter = x;
            
            // add zeros to least significant side for rule of multipliction
            for ( size_t l = 0; l < i; l++) {
                    inter.insert(inter.begin(), 0);
            }
            
            // add vectors together
            vector<bool> temp;
            temp=addingBinary(z,inter);
            z=temp;

            // show values for debugging
            if ( debug) {
                cout<<"Intermedite: ";
                printVector(inter);
                cout<<"\n";
                cout<<"Z: ";
                printVector(z);
                cout<<"\n";
            }
        }
    }

    return z;
}

// multiples binary vectors with divide and conquer
// [[Rcpp::export]]
vector<bool> multiply(vector<bool> & x, vector<bool> & y ,bool debug) {

    vector<bool> z;
    
    // makes vectors same size
    paddingVector(x, y);

    
    // returns xy if size is 1
    if ( x.size() == 1 && y.size() == 1 ) {
        z.push_back(0);
        z[0] = x[0] & y[0];
        return z; 
    }
    
    // makes size even 
    if ( x.size() % 2 != 0 ) {
        x.push_back(0);
        y.push_back(0);
    }

    // vectors for right and left bits
    vector<bool> xR;
    vector<bool> xL;
    vector<bool> yR;
    vector<bool> yL;

    int xRsize, xLsize, yRsize, yLsize;

    // does flooring and ceiling on sizes
    xRsize = floor(x.size()/2.0);
    yRsize = floor(y.size()/2.0);
    xLsize = ceil(y.size()/2.0);
    yLsize = ceil(y.size()/2.0);

    xR.assign(xRsize, 0);
    xL.assign(xLsize, 0);
    yR.assign(yRsize, 0);
    yL.assign(yLsize, 0);
    
    // calculates rightmost bits and leftmost bits
    for ( size_t i = 0; i < x.size() && i < y.size(); i++ ) {
        if ( (int)i < xRsize ) {
            xR[i] = x[i];
        }
        else {
            xL[i - xRsize] = x[i];
        }
        
        if ( (int)i < yRsize ) {
            yR[i] = y[i];
        }
        else {
            yL[i - yRsize] = y[i];
        }
    }
    
    vector<bool> P1 = multiply(xL, yL,0);
    vector<bool> P2 = multiply(xR, yR,0);
    vector<bool> temp = addingBinary(xL,xR);
    vector<bool> temp1 = addingBinary(yL,yR);
    vector<bool> P3 = multiply( temp, temp1,0);

    xR.clear();
    xL.clear();
    yR.clear();
    yL.clear();

    // Starting to do final return, but must break apart answer because C++ 
    // does not support changing temporary vectors.
    temp.clear();
    temp = P1;

    // multiple by 2
    for ( size_t i = 0; i < x.size(); i++ ) {
        temp.insert(temp.begin(),0);
    }
    
    temp1.clear();
    temp1 = subtractingBinary(P3,P1);
     
    vector<bool> temp2 = subtractingBinary(temp1,P2);

    // multiple by 2^n/2
    for ( size_t i = 0; i < x.size()/2; i++ ) {
        temp2.insert(temp2.begin(),0);
    }
    
    temp1.clear();
    temp1 = addingBinary( temp2, P2 );
    z = addingBinary( temp, temp1 );
    
    return z;
}


// test funtion for muliply functions
bool test(vector<bool> (* mul)(vector<bool> &x, vector<bool> &y, bool debug)) {

    bool passed = true;

    // Example 1
    // TO-DO: if failed, print out error message
    vector<bool> byte1 = {1,1,1,1};
    vector<bool> byte2 = {1,1};
    vector<bool> answer= { 1,0,1,1,0,1, };

    vector<bool> result = mul(byte2,byte1,0);
    
    paddingVector(result, answer);

    for ( size_t i = 0; i < result.size(); i++ ) {
        if ( result[i] != answer[i] ) {
            passed = false;
            cout<<"Error, Test Case 1 has Failed"<<endl;
            cout<<"Result: ";
            printVector(result);
            cout<<endl;
            cout<<"Answer: ";
            printVector(answer);
            cout<<endl;
            break;
        }
    }
    
    // Example 2
    // TO-DO: if failed, print out error message
    byte1 = {1,0,1,1,1,1,1,1};
    byte2 = {1,1,1,1,1,1,1,1};
    answer= {1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0};

    result = mul(byte1,byte2,0);

    paddingVector(result, answer);

    for ( size_t i = 0; i < result.size(); i++ ) {
        if ( result[i] != answer[i] ) {
            passed = false;
            cout<<"Error, Test Case 2 has Failed"<<endl;
            cout<<"Result: ";
            printVector(result);
            cout<<endl;
            cout<<"Answer: ";
            printVector(answer);
            cout<<endl;
            break;
        }
    }
    
    // Example 3
    // TO-DO: if failed, print out error message

    byte1 = {1,1,1};
    byte2 = {1,1,1,1,0,0,0,1};
    answer= {1,0,0,1,0,1,1,1,1,1};

    result = mul(byte1,byte2,0);
    
    paddingVector(result, answer);

    for ( size_t i = 0; i < result.size(); i++ ) {
        if ( result[i] != answer[i] ) {
            passed = false;
            cout<<"Error, Test Case 3 has Failed"<<endl;
            cout<<"Result: ";
            printVector(result);
            cout<<endl;
            cout<<"Answer: ";
            printVector(answer);
            cout<<endl;
            break;
        }
    }

    // Example 4
    // TO-DO: if failed, print out error message

    byte1 = {1,0,0,1,0,1,0,1,0,1};
    byte2 = {1,0,0,1,1,1,1,0,1,0,1,0,0,1};
    answer= {1,0,0,0,0,1,1,1,0,1,1,1,0,1,0,1,1,1,0,0,0,1,1};

    result = mul(byte1,byte2,0);

    paddingVector(result, answer);

    
    for ( size_t i = 0; i < result.size(); i++ ) {
        if ( result[i] != answer[i] ) {
            passed = false;
            cout<<"Error, Test Case 4 has Failed"<<endl;
            cout<<"Result: ";
            printVector(result);
            cout<<endl;
            cout<<"Answer: ";
            printVector(answer);
            cout<<endl;
            break;
        }
    }

    
    // Example 5
    // TO-DO: if failed, print out error message

    byte1 = {0,1,0,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,0,1,0,1,0,1,1,1,1};
    byte2 = {1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,0,0,1};
    answer= {0,1,0,1,1,1,0,0,0,1,1,1,0,0,1,0,1,1,1,0,0,1,0,1,1,0,0,1,0,1,0,0,1,1,0,0,0,0,1,1,1,0,0,1,0,0,0,1,0};

    result = mul(byte1,byte2,0);

    paddingVector(result, answer);

    for ( size_t i = 0; i < result.size(); i++ ) {
        if ( result[i] != answer[i] ) {
            passed = false;
            cout<<"Error, Test Case 4 has Failed"<<endl;
            cout<<"Result: ";
            printVector(result);
            cout<<endl;
            cout<<"Answer: ";
            printVector(answer);
            cout<<endl;
            break;
        }
    }

    
    return passed;
}

// [[Rcpp::export]]
bool testall() {
    // test the iterative solution:
    bool passed = test(multiply_itr);
    // TO-DO: print out success / failure messages
    if ( passed ) {
        cout<<"Tests have passed for multiply_itr()"<<endl<<endl;
    }
    else{
        cout<<"Tests have failed for multiply_itr()"<<endl<<endl;
    }
    // test the divide-and-conquer solution:
    passed = test(multiply);
    // TO-DO: print out success / failure messages
    if ( passed ) {
        cout<<"Tests have passed for multiply()"<<endl<<endl;
    }
    else{
        cout<<"Tests have failed for multiply()"<<endl<<endl;
    }
    return passed;
}

int main () {

    testall();

    return 0;
}

//--------------------------------------------------------------

/*** R
library(ggplot2)
library(gridExtra)
library(grid)
if ( !testall() ) { quit(status=1)}

n <- 10
runtimeI <- vector(length = n)
runtimeR <- vector(length = n)
ns <- 500*(1:n)
for ( k in 1:n ) {
  x <- sample(c(0,1), ns[k], replace=TRUE)
  y <- sample(c(0,1), ns[k], replace=TRUE)
  runtimeR[k] = system.time(multiply(x, y,0))[["user.self"]]
  runtimeI[k] = system.time(multiply_itr(x, y,0))[["user.self"]]
}

cI <- .01
count <- 6
diff <- (cI * ns[count]^2 )  - runtimeI[count]
while ( diff > .01 ) {
  cI <- cI - .00000001
  diff <- (cI * ns[count]^2 )  - runtimeI[count]
}

theoI <- vector(length = n)
for ( k in 1:n ) {
  theoI[k] = cI * ns[k]^2
}

logBase2Of3 <- log(3,base=2)

cR <- .0001
count <- 6
diff <- (cR * ns[count]^logBase2Of3 )  - runtimeR[count]
while ( diff > .01 ) {
  cR <- cR - .000000001
  diff <- (cR * ns[count]^logBase2Of3 )  - runtimeR[count]
}

theoR <- vector(length = n)

for ( k in 1:n ) {
  theoR[k] = cR * ns[k]^logBase2Of3
}


diagonalI <-data.frame(
  graphType = c("linear","linear",rep(c("empircal"), each=10), rep(c("theorical"), each=10)),
  x = c(ns[1], ns[10],rep(ns),rep(ns)),
  y = c(runtimeI[1], runtimeI[10], rep(runtimeI),rep(theoI))
)

diagonalR <-data.frame(
  graphType = c("linear","linear",rep(c("empircal"), each=10), rep(c("theorical"), each=10)),
  x = c(ns[1], ns[10],rep(ns), rep(ns)),
  y = c(runtimeR[1], runtimeR[10], rep(runtimeR), rep(theoR))
)



p <- ggplot(diagonalI,aes(x,y, group=graphType)) +
  geom_point(aes(color=graphType, shape=graphType)) +
  geom_line(aes(linetype=graphType, color=graphType)) + 
  scale_linetype_manual(values=c("dashed","solid", "dotted"))  


p <- p + labs(x="n-bit", y="Runtime")

p <- p + ggtitle("Multiplication of n-bit numbers\nIterative")

l <- ggplot(diagonalR,aes(x,y, group=graphType)) +
  geom_point(aes(color=graphType, shape=graphType)) +
  geom_line(aes(linetype=graphType, color=graphType)) + 
  scale_linetype_manual(values=c("dashed","solid", "dotted")) 


l <- l + labs(x="n-bit", y="Runtime")
l <- l + ggtitle("Multiplication of n-bit numbers\nDivide And Conquer")


grid.arrange(p, l, nrow = 1)

 */