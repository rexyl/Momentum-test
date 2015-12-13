//
//  main.cpp
//  Momentum_test
//
//  Created by Lu Yang on 12/5/15.
//  Copyright © 2015 Lu Yang. All rights reserved.

#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <cfloat>
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
    //double t = avg / (sqrt( (variance)/data.size() ));
    double t = avg / sqrt(variance);
    return t;
}
struct stock_point {
    string code;
    double quater_price;
    double capitalization;
    double increase;
    stock_point(){
        code = "";
        quater_price = 0;
        capitalization = 0;
        increase = 0;
    }
};
typedef map<string,stock_point> timepoint;
vector<timepoint> data;

vector<string> time_p;
vector<double> winner_next_big;
vector<double> loser_next_big;
vector<double> big_diff;
vector<double> winner_next_mid;
vector<double> loser_next_mid;
vector<double> mid_diff;
vector<double> winner_next_small;
vector<double> loser_next_small;
vector<double> small_diff;
bool cmp_capital(pair<stock_point, double> t1,pair<stock_point, double> t2){
    return t1.second<t2.second;
}
bool cmp_increse(pair<stock_point, double> t1,pair<stock_point, double> t2){
    return t1.first.increase<t2.first.increase;
}
int main(int argc, const char * argv[]) {
    // insert code here...
    vector<double> v= {1.1,3.1,4.5};
    vector<double> v1= {1.5,2.1,8.5};
    double a,var,t;
    avg_var(v, a, var);
    t = t_test(v);
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
    double num = 0,last =0;
    while(getline(ifs, line, '\n')){
        last = 0;
        ss.str(string());
        ss.clear();
        ss<<line;
        ss>>code;
        if(!ss){
            return 0;
        }
        cnt = 0;
        while(cnt<60 && ss>>num){
            data[cnt][code].code = code;
            data[cnt][code].quater_price = num;
            if (last < FLT_EPSILON ) {
                data[cnt][code].increase = NAN;
            }
            else {
                data[cnt][code].increase = (num - last)/last;
            }
            cnt++;
            last = num;
        }
        cnt = 0;
        while (cnt<60 && ss>>num) {
            data[cnt][code].capitalization = num;
            cnt++;
        }
    }
    //cout<<data[1]["000301.SZ"].increase<<endl;
    vector<pair<stock_point, double>> cap;
    
    winner_next_small.clear();
    winner_next_mid.clear();
    winner_next_big.clear();
    loser_next_big.clear();
    loser_next_mid.clear();
    loser_next_small.clear();
    small_diff.clear();
    mid_diff.clear();
    big_diff.clear();
    int gap1 = 1,gap2=1;
    for(int period = 1;period<59;period = period + gap1){
        if(period + gap2 > 59)
            break;
        cap.clear();
        for (auto &x:data[period]) {
            if(x.second.capitalization>FLT_EPSILON && x.second.increase==x.second.increase)
                cap.push_back({x.second,x.second.capitalization});
        }
        if (cap.empty()) {
            continue;
        }
//        for(auto x:cap){
//            cout<<x.first.code<<endl;
//        }
        sort(cap.begin(), cap.end(), cmp_capital);
        vector<pair<stock_point, double>> small(cap.begin(),cap.begin()+cap.size()/3);
        vector<pair<stock_point, double>> mid(cap.begin()+cap.size()/3,cap.begin()+2*cap.size()/3);
        vector<pair<stock_point, double>> big(cap.begin()+2*cap.size()/3,cap.end());
        sort(big.begin(), big.end(), cmp_increse);
        sort(mid.begin(), mid.end(), cmp_increse);
        sort(small.begin(), small.end(), cmp_increse);
//        for(auto x:small){
//            cout<<x.first.code<<endl;
//        }
        double sum1 = 0,sum2 = 0;
        size_t size = big.size()/10;
        for(int i=0;i<size;i++){
            sum1 += data[period+1][big[i].first.code].increase;
            sum2 += data[period+1][big[big.size()-1-i].first.code].increase;
        }
        winner_next_big.push_back(sum2/size);
        loser_next_big.push_back(sum1/size);
        big_diff.push_back((sum2-sum1)/size);
        size = mid.size()/10;
        for(int i=0,sum1=sum2=0;i<size;i++){
            sum1 += data[period+1][mid[i].first.code].increase;
            sum2 += data[period+1][mid[mid.size()-1-i].first.code].increase;
        }
        winner_next_mid.push_back(sum2/size);
        loser_next_mid.push_back(sum1/size);
        mid_diff.push_back((sum2-sum1)/size);
        size = small.size()/10;
        for(int i=0,sum1=sum2=0;i<size;i++){
            sum1 += data[period+1][small[i].first.code].increase;
            sum2 += data[period+1][small[small.size()-1-i].first.code].increase;
        }
        winner_next_small.push_back(sum2/size);
        loser_next_small.push_back(sum1/size);
        small_diff.push_back((sum2-sum1)/size);
        //cout<<"worst is "<<sum1/10<<"\nbest is "<<sum2/10<<endl;
    }
    //cout<<time_p[0]<<endl;
    //cout<<data[0]["600004.SH"].quater_price<<" "<<data[0]["600000`.SH"].capitalization<<endl;
//    for (int i=0; i<60; i++) {
//        //cout<<data[i]["600008.SH"].increase<<endl;
//        cout<<winner_next_small[i]<<endl;
//    }
    //cout<<winner_next_small[1]<<endl;
    for(auto x:big_diff){
        cout<<x<<endl;
    }
//    cout<<"In ("<<gap1*3<<gap2*3<<")\n";
//    cout<<"     T-test for big: "<<t_test(big_diff)<<endl;
//    cout<<"     T-test for mid: "<<t_test(mid_diff)<<endl;
//    cout<<"     T-test for small: "<<t_test(small_diff)<<endl;
    return 0;
}
