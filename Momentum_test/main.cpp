//
//  main.cpp
//  Momentum_test
//
//  Created by Lu Yang on 12/5/15.
//  Copyright © 2015 Lu Yang. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <string>
using namespace std;
void avg_var(vector<double> &data, double &avg,double &variance){
    avg = variance = 0;
    for (auto x:data) {
        avg += x;
        variance += x*x;
    }
    avg = avg / data.size();
    variance = (variance/data.size()) - avg*avg;
    return;
}

double t_test(vector<double> &data){
    double avg,variance;
    avg_var(data, avg, variance);
    double t = avg / sqrt( (variance)/data.size() );
    return t;
}
int main(int argc, const char * argv[]) {
    // insert code here...
    vector<double> v= {1.1,3.1,4.5};
    vector<double> v1= {1.5,2.1,8.5};
    double a,var,t;
    avg_var(v, a, var);
    t = t_test(v);
    //cout << a<<" "<<var<<endl;
    //cout<<t<<endl;
    ifstream ifs("/Users/Rex/Desktop/Momentum_test/simple.txt");
    if(!ifs){
        cout<<"File open failed\n";
    }
    string trash;
    getline(ifs, trash, '\n');
    getline(ifs, trash, '\n');
    cout<<trash<<endl;
    return 0;
}
