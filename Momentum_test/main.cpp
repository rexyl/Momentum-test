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
#include <map>
#include <sstream>
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
struct stock_point {
    string code;
    double quater_price;
    double capitalization;
    stock_point(){
        code = "";
        quater_price = 0;
        capitalization = 0;
    }
};
typedef map<string,stock_point> timepoint;
vector<timepoint> data;
vector<string> time_p;
int main(int argc, const char * argv[]) {
    // insert code here...
    vector<double> v= {1.1,3.1,4.5};
    vector<double> v1= {1.5,2.1,8.5};
    double a,var,t;
    avg_var(v, a, var);
    t = t_test(v);
    //cout << a<<" "<<var<<endl;
    //cout<<t<<endl;
    ifstream ifs("/Users/Rex/Desktop/Momentum_test/english_version.txt");
    stringstream ss;
    if(!ifs){
        cout<<"File open failed\n";
    }
    string line,code,tmp;
    getline(ifs, line, '\n');
    getline(ifs, line, '\n');
    int cnt = 1;
    ss<<line;ss>>tmp;
    while(ss&&cnt++<=60){
        ss>>tmp;
        if(tmp.size()<8) continue;
        time_p.push_back(tmp);
        timepoint t;
        data.push_back(t);
    }
    double num = 0;
    while(getline(ifs, line, '\n')){
        //cout<<line<<endl;
        ss.str(string());
        ss.clear();
        //stringstream sss;
        ss<<line;
        ss>>code;
        //cout<<code<<" is "<<num<<endl;
        if(!ss){
            return 0;
        }
        cnt = 0;
        while(cnt<60 && ss>>num){
            data[cnt][code].code = code;
            data[cnt][code].quater_price = num;
            cout<<num<<endl;
            cnt++;
        }
        cnt = 0;
        while (cnt<60 && ss>>num) {
            data[cnt][code].capitalization = num;
            cnt++;
        }
    }
    cout<<data[2]["600005.SH"].quater_price<<" "<<data[2]["600005.SH"].capitalization<<endl;
    //cout<<line<<endl;
    return 0;
}
