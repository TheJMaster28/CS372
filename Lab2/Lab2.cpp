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
            for ( size_t k = 0; k < x.size(); k++) {
                for ( size_t l = 0; l < x.size(); l++) {
                    inter[l] = x[l];
                } 
            }
            
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

// [[Rcpp::export]]
vector<bool> multiply(vector<bool> & x, vector<bool> & y ,bool debug) {

    vector<bool> z;
    
    paddingVector(x, y);

    
    if ( x.size() == 1 && y.size() == 1 ) {
        z.push_back(0);
        z[0] = x[0] & y[0];
        return z; 
    }

    if ( x.size() % 2 != 0 ) {
        x.push_back(0);
        y.push_back(0);
    }

    vector<bool> xR;
    vector<bool> xL;
    vector<bool> yR;
    vector<bool> yL;

    int xRsize, xLsize, yRsize, yLsize;

    xRsize = floor(x.size()/2.0);
    yRsize = floor(y.size()/2.0);
    xLsize = ceil(y.size()/2.0);
    yLsize = ceil(y.size()/2.0);

    xR.assign(xRsize, 0);
    xL.assign(xLsize, 0);
    yR.assign(yRsize, 0);
    yL.assign(yLsize, 0);
    
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

    temp.clear();
    temp = P1;

    // 
    for ( size_t i = 0; i < x.size(); i++ ) {
        temp.insert(temp.begin(),0);
    }
    
    // 
    temp1.clear();
    temp1 = subtractingBinary(P3,P1);
    
    // 
    vector<bool> temp2 = subtractingBinary(temp1,P2);

    // 
    for ( size_t i = 0; i < x.size()/2; i++ ) {
        temp2.insert(temp2.begin(),0);
    }
    
    // 
    temp1.clear();
    // adding problem 36 + 9
    temp1 = addingBinary( temp2, P2 );
    
    // 
    z = addingBinary( temp, temp1 );
    // z= temp1;
    
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
    // testingMuliply();
    

    testall();

    return 0;
}


// void testingMuliply() {
//     string byte1;
//     string byte2;
//     string answer;
//     bool passed = true;
//     bool debug;
    
//     // get input from user
//     cout<<"Enter Byte 1: "<<endl;
//     cin>>byte1;

//     cout<<"Enter Byte 2: "<<endl;
//     cin>>byte2;

//     cout<<"Enter Answer: "<<endl;
//     cin>>answer;

//     cout<<"Debug?"<<endl;
//     cin>>debug;

//     vector<bool> x,y,z;

//     // convert string to boolean vector
//     for( int i = byte1.length()-1; i >= 0; i-- ) {
//         x.push_back( byte1.at(i) == '1' );
//     }

//     for ( int i = byte2.length()-1; i >= 0; i--) {
//         y.push_back( byte2.at(i) == '1' );
//     }

//     for ( int i = answer.length()-1; i >= 0; i--) {
//         z.push_back( answer.at(i) == '1' );
//     }
    
//     // do Muliply_itr function
//     vector<bool> result = multiply_itr(x,y,debug);

//     // make both input vector and result vector same size for easy comparing
//     while ( result.size() != z.size() ) {
//         if ( z.size() > result.size() ) {
//             result.push_back(0);
//         }
//         else {
//             z.push_back(0);
//         }
//     }

//     // compare values for mismatch bits
//     for ( int i = 0; i < z.size(); i++ ) {
//         if ( result[i] != z[i] ) {
//             passed = false;
//             break;
//         }
//     }

//     if ( passed ) {
//         cout<<"muliply_itr has Passed!"<<endl;
//     }
//     else {
//         cout<<"muliply_itr has Failed"<<endl;
//         cout<<"Result: ";
//         printVector(result);
//         cout<<endl;
//     }
    
// }

//--------------------------------------------------------------

/*** R
library(ggplot2)
library(gridExtra)
library(grid)

n <- 10
runtimeI <- vector(length = n)
runtimeR <- vector(length = n)
ns <- vector(length = n)
for ( k in 1:n ) {
  ns[k] = n*k*k
  x <- sample(c(0,1), ns[k], replace=TRUE)
  y <- sample(c(0,1), ns[k], replace=TRUE)
  runtimeR[k] = system.time(multiply(x, y,0))[["user.self"]]
  runtimeI[k] = system.time(multiply_itr(x, y,0))[["user.self"]]
}
theo <- vector(length = n)

diagonalI <-data.frame(
  graphType = c("linear","linear",rep(c("empircal"), each=10), rep(c("theorical"), each=10)),
  x = c(ns[1], ns[10],rep(ns), rep(ns)),
  y = c(runtimeI[1], runtimeI[10], rep(runtimeI), rep(theo))
)

diagonalR <-data.frame(
  graphType = c("linear","linear",rep(c("empircal"), each=10), rep(c("theorical"), each=10)),
  x = c(ns[1], ns[10],rep(ns), rep(ns)),
  y = c(runtimeR[1], runtimeR[10], rep(runtimeR), rep(theo))
)



p <- ggplot(diagonalI,aes(x,y, group=graphType)) + geom_point(aes(color=graphType, shape=graphType))+ geom_line(aes(linetype=graphType, color=graphType)) + scale_linetype_manual(values=c("dashed","solid", "dotted"))
p + labs(x="n-bit", y="Runtime")
p + ggtitle("Multiplication of n-bit numbers")

l <- ggplot(diagonalR,aes(x,y, group=graphType)) + geom_point(aes(color=graphType, shape=graphType))+ geom_line(aes(linetype=graphType, color=graphType)) + scale_linetype_manual(values=c("dashed","solid", "dotted"))
l + labs(x="n-bit", y="Runtime")
l + ggtitle("Multiplication of n-bit numbers")


grid.arrange(p, l, nrow = 1)

 */

/* #plot(linearx, lineary, type="b", xlab="n", ylab="runtime (second)", main = "Multiply_itr")
#plot(ns, runtimeI, type="b", xlab="n", ylab="runtime (second)", main = "Multiply_itr")
#grid(col="blue")

#plot(ns, runtimeR, type="b", xlab="n", ylab="runtime (second)", main = "Multiply")
#grid(col="blue")
 */