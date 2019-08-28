// Jeffrey Lansford
// Lab2
#include <iostream>
#include <vector>
using namespace std;

vector<bool> muliply_itr(vector<bool> & x, vector<bool> & y) {
    vector<bool> z;
    vector<bool> inter;
    bool carry;
    // fills vector with zeros to inlize vector
    z.assign(x.size() + y.size(), 0);
    inter.assign(z.size(), 0);
    for ( int i = 0; i < y.size(); i++ ) {
        if ( y[i] ) {
            for ( int k = 0; k < x.size(); k++) {
                for ( int l = 0; l < x.size(); l++) {
                    inter[l] = x[l];
                }
                for ( int l = 0; l < k; l++) {
                    inter.insert(inter.begin(), 0);
                }
                
            }
            for ( int k = 0; k < z.size(); k++) {
                z[k] = ((z[k] ^ inter[k] ^ carry ));
                carry = (( z[k] & inter[k] ) | (z[k] & carry)) | (inter[k] & carry);
            }
        }
        else {
            for ( int k = 0; k < x.size(); k++) {
                for ( int l = 0; l < x.size(); l++) {
                    inter[l] = 0;
                }
                for ( int l = 0; l < k; l++) {
                    inter.insert(inter.begin(), 0);
                }
                
            }
            for ( int k = 0; k < z.size(); k++) {
                z[k] = ((z[k] ^ inter[k] ^ carry ));
                carry = (( z[k] & inter[k] ) | (z[k] & carry)) | (inter[k] & carry);
            }

        }
       
    }

    return z;
}


int main () {
    vector<bool> x = { 0,1,0,1};
    vector<bool> y = {1,1};
    vector<bool> answer = { 0,1,1,1,0};

    vector<bool> result = muliply_itr(x,y);
    cout<<"Result: ";
    for ( int i = result.size(); i > 0; i--) {
        cout<<result[i]<<" ";
    }
    cout<<endl;
}