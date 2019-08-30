// Jeffrey Lansford
// Lab2
#include <iostream>
#include <vector>
#include <string>
using namespace std;


void printVector( vector<bool>  &a) {
    for ( int i = a.size(); i >= 0; i--) {
        cout<<a[i]<<' ';
    }
}

// adds two binary numbers in a boolean vector
vector<bool> addingBinary( vector<bool> &x, vector<bool> &y) {
    vector<bool> z;
    bool carry=0;
    for ( int k = 0; ( k < x.size() || k < y.size() ); k++) {
        z.push_back(((x[k] ^ y[k] ^ carry )));
        carry = (( x[k] & y[k] ) | (x[k] & carry)) | (y[k] & carry);
    }
    return z;

}

// multiples to boolean vectors itleriave
vector<bool> multiply_itr(vector<bool> & x, vector<bool> & y ,bool debug) {
    vector<bool> z;
    vector<bool> inter;
    bool carry;
    
    // fills vector with zeros to inlize vector
    z.assign(x.size() + y.size(), 0);
    
    // goes through each bit from y byte
    for ( int i = 0; i < y.size(); i++ ) {
        
        // makes intermedite value zero
        inter.assign(x.size(), 0);
        
        // if y's bit is one, then copy x to intermedite
        if ( y[i] ) {
            for ( int k = 0; k < x.size(); k++) {
                for ( int l = 0; l < x.size(); l++) {
                    inter[l] = x[l];
                } 
            }
            
            // add zeros to least significant side for rule of multipliction
            for ( int l = 0; l < i; l++) {
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


vector<bool> multiply(vector<bool> & x, vector<bool> & y ,bool debug) {
    vector<bool> z;
    
    return z;
}


// test funtion for muliply_itr
void testingMuliply() {
    string byte1;
    string byte2;
    string answer;
    bool passed = true;
    bool debug;
    
    // get input from user
    cout<<"Enter Byte 1: "<<endl;
    cin>>byte1;

    cout<<"Enter Byte 2: "<<endl;
    cin>>byte2;

    cout<<"Enter Answer: "<<endl;
    cin>>answer;

    cout<<"Debug?"<<endl;
    cin>>debug;

    vector<bool> x,y,z;

    // convert string to boolean vector
    for( int i = byte1.length()-1; i >= 0; i-- ) {
        x.push_back( byte1.at(i) == '1' );
    }

    for ( int i = byte2.length()-1; i >= 0; i--) {
        y.push_back( byte2.at(i) == '1' );
    }

    for ( int i = answer.length()-1; i >= 0; i--) {
        z.push_back( answer.at(i) == '1' );
    }
    
    // do Muliply_itr function
    vector<bool> result = multiply_itr(x,y,debug);

    // make both input vector and result vector same size for easy comparing
    while ( result.size() != z.size() ) {
        if ( z.size() > result.size() ) {
            result.push_back(0);
        }
        else {
            z.push_back(0);
        }
    }

    // compare values for mismatch bits
    for ( int i = 0; i < z.size(); i++ ) {
        if ( result[i] != z[i] ) {
            passed = false;
            break;
        }
    }

    if ( passed ) {
        cout<<"muliply_itr has Passed!"<<endl;
    }
    else {
        cout<<"muliply_itr has Failed"<<endl;
        cout<<"Result: ";
        printVector(result);
        cout<<endl;
    }
    
}

bool test(vector<bool> (* mul)(vector<bool> &x, vector<bool> &y, bool debug)) {

    bool passed = true;

    // Example 1
    // TO-DO: if failed, print out error message
    vector<bool> byte1 = {1,1};
    vector<bool> byte2 = {1,0,1};
    vector<bool> answer= {1,1,1,1};

    vector<bool> result = mul(byte1,byte2,0);

    while ( result.size() != answer.size() ) {
            if ( answer.size() > result.size() ) {
                result.push_back(0);
            }
            else {
                answer.push_back(0);
            }
    }

    for ( int i = 0; i < result.size(); i++ ) {
        if ( result[i] != answer[i] ) {
            passed = false;
            cout<<"Error, Test Case 1 has Failed"<<endl;
            break;
        }
    }
    
    // Example 2
    // TO-DO: if failed, print out error message
    byte1 = {1,0,0,1,0,1,0,1,0,1};
    byte2 = {1,0,0,1,1,1,1,0,1,0,1,0,0,1};
    answer= {1,0,0,0,0,1,1,1,0,1,1,1,0,1,0,1,1,1,0,0,0,1,1};

    result = mul(byte1,byte2,0);

    while ( result.size() != answer.size() ) {
            if ( answer.size() > result.size() ) {
                result.push_back(0);
            }
            else {
                answer.push_back(0);
            }
    }

    for ( int i = 0; i < result.size(); i++ ) {
        if ( result[i] != answer[i] ) {
            passed = false;
            cout<<"Error, Test Case 2 has Failed"<<endl;
            break;
        }

    }
    
    // Example 3
    // TO-DO: if failed, print out error message

    // Example 4
    // TO-DO: if failed, print out error message
    
    // Example 5
    // TO-DO: if failed, print out error message
    return passed;
}

bool testall() {
    // test the iterative solution:
    bool passed = test(multiply_itr);
    // TO-DO: print out success / failure messages
    if ( passed ) {
        cout<<"Tests have passed for multiply_itr()"<<endl;
    }
    else{
        cout<<"Tests have failed for multiply_itr()"<<endl;
    }
    // test the divide-and-conquer solution:
    passed = test(multiply);
    // TO-DO: print out success / failure messages
    if ( passed ) {
        cout<<"Tests have passed for multiply()"<<endl;
    }
    else{
        cout<<"Tests have failed for multiply()"<<endl;
    }
    return passed;
}

int main () {
    // testingMuliply();
    testall();
    return 0;
}