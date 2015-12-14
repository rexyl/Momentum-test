//
//  main.cpp
//  Momentum_test
//  testing for the momentum effect in China using data from 2000 to 2014,
//  separately for large, small, and medium firms.

//  Created by Lu Yang on 12/5/15.
//  All rights reserved.

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
    double t = avg /sqrt( (variance)/data.size() );
    return t;
}
struct stock_point {
    string code;
    double quater_price;
    double capitalization;
    double increase1;
    double increase2;
    stock_point(){
        code = "";
        quater_price = 0;
        capitalization = 0;
        increase1 = 0;
        increase2 = 0;
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
    return t1.first.increase1<t2.first.increase1;
}
void output_format(vector<double> v){
    double sum=0;
    for(auto x:v){
        sum+=x;
    }
    cout<<sum/v.size()<<",("<<t_test(v)<<")";
}
int main_fake(int ind) {
    ifstream ifs("/Users/Rex/Desktop/Momentum_test/monthly.txt");
    stringstream ss;
    if(!ifs){
        cout<<"File open failed\n";
    }
    string line,code,tmp;
    getline(ifs, line, '\n');
    getline(ifs, line, '\n');
    int cnt = 1;
    ss<<line;ss>>tmp;
    while(ss&&cnt++<=180){
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
        while(cnt<180 && ss>>num){
            data[cnt][code].code = code;
            data[cnt][code].quater_price = num;
//            if (last < FLT_EPSILON ) {
//                data[cnt][code].increase = NAN;
//            }
//            else {
//                data[cnt][code].increase = (num - last)/last;
//            }
            data[cnt][code].increase1 = NAN;
            data[cnt][code].increase2 = NAN;
            cnt++;
            last = num;
        }
        cnt = 0;
        while (cnt<180 && ss>>num) {
            data[cnt][code].capitalization = num;
            cnt++;
        }
    }
    
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
    int gap1,gap2;
    for(gap1 = 3;gap1!=15;gap1=gap1+3){
        for(gap2 = 3;gap2!=15;gap2=gap2+3){
            for (int i=0; i<data.size(); i++) {
                if(i-gap1>=0 or i-gap2>=0){
                    for(auto &x:data[i]){
                        if(i-gap1>=0 &&data[i-gap1][x.second.code].quater_price != 0)
                            x.second.increase1 = (x.second.quater_price/data[i-gap1][x.second.code].quater_price) -1;
                        if(i-gap2>=0 &&data[i-gap2][x.second.code].quater_price != 0)
                            x.second.increase2 = (x.second.quater_price/data[i-gap2][x.second.code].quater_price) -1;
                    }
                }
            }
            
            for(int period = 1;period<179;period = period + 1){
                if(period + gap2 > 179)
                    break;
                cap.clear();
                for (auto &x:data[period]) {
                    if(x.second.capitalization>FLT_EPSILON && x.second.increase1==x.second.increase1)
                        cap.push_back({x.second,x.second.capitalization});
                }
                if (cap.empty()) {
                    continue;
                }
                sort(cap.begin(), cap.end(), cmp_capital);
                vector<pair<stock_point, double>> small(cap.begin(),cap.begin()+cap.size()/3);
                vector<pair<stock_point, double>> mid(cap.begin()+cap.size()/3,cap.begin()+2*cap.size()/3);
                vector<pair<stock_point, double>> big(cap.begin()+2*cap.size()/3,cap.end());
                sort(big.begin(), big.end(), cmp_increse);
                sort(mid.begin(), mid.end(), cmp_increse);
                sort(small.begin(), small.end(), cmp_increse);
                double sum1 = 0,sum2 = 0;
                size_t size = big.size()/10;
                for(int i=0;i<size;i++){
                    sum1 += data[period+gap2][big[i].first.code].increase2;
                    sum2 += data[period+gap2][big[big.size()-1-i].first.code].increase2;
                }
                winner_next_big.push_back(sum2/size/gap2);
                loser_next_big.push_back(sum1/size/gap2);
                big_diff.push_back((sum2-sum1)/size/gap2);
                size = mid.size()/10;
                for(int i=0,sum1=sum2=0;i<size;i++){
                    sum1 += data[period+gap2][mid[i].first.code].increase2;
                    sum2 += data[period+gap2][mid[mid.size()-1-i].first.code].increase2;
                }
                winner_next_mid.push_back(sum2/size/gap2);
                loser_next_mid.push_back(sum1/size/gap2);
                mid_diff.push_back((sum2-sum1)/size/gap2);
                size = small.size()/10;
                for(int i=0,sum1=sum2=0;i<size;i++){
                    sum1 += data[period+gap2][small[i].first.code].increase2;
                    sum2 += data[period+gap2][small[small.size()-1-i].first.code].increase2;
                }
                winner_next_small.push_back(sum2/size/gap2);
                loser_next_small.push_back(sum1/size/gap2);
                small_diff.push_back((sum2-sum1)/size/gap2);
            }
            if(ind==0){
                output_format(small_diff);
                cout<<"\t";
                output_format(mid_diff);
                cout<<"\t";
                output_format(big_diff);
                cout<<"\n";
            }
            if(ind==1){
                output_format(winner_next_small);
                cout<<"\t";
                output_format(winner_next_mid);
                cout<<"\t";
                output_format(winner_next_big);
                cout<<"\n";
            }
            if(ind==2){
                output_format(loser_next_small);
                cout<<"\t";
                output_format(loser_next_mid);
                cout<<"\t";
                output_format(loser_next_big);
                cout<<"\n";
            }
        }
    }
    ifs.close();
    return 0;
}
int main(){
    for (int i=0; i<3; i++) {
        main_fake(i);
        cout<<"\n\n";
    }
    return 0;
}
