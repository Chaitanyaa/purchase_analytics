#include <bits/stdc++.h>
#include<fstream>
#include <iostream>
#include <iomanip>
#include<math.h>
using namespace std;

void readFile(string p_filepath,fstream &p_fin){
    try{
        p_fin.open(p_filepath,ios::in);
        if(!p_fin) throw "Error opening file! for read" ;
    }
    catch( const char* cstr )
    {
        std::cerr << cstr << '\n' ;
    }
}

void headerMap(fstream &p_fin,unordered_map<string,int> &p_headmap,int &p_headerlinecount){
    string m_row,m_col;
    getline(p_fin,m_row);
    stringstream s(m_row);
    while(getline(s,m_col,',')){
        p_headmap[m_col] = p_headerlinecount++;
    }
}

vector<string> splitLine(fstream &p_fin){
    vector<string> m_dataitems;
    string m_row,m_col;
    getline(p_fin,m_row);
    stringstream s(m_row);
    while(getline(s,m_col,',')){
        m_col.erase(remove(m_col.begin(),m_col.end(),'\"'),m_col.end());
        if(m_col.size()>0){
            m_dataitems.push_back(m_col);
        }
    }
    return m_dataitems;
}
void writeFile(string p_filepath,fstream &p_fout, map<int,pair<int,int>> &p_dept_orders_map){
    int m_count;
    float m_percentage;
    try{
        p_fout.open(p_filepath,ios::out);
        if(!p_fout) throw "Error opening file for write!" ;
        p_fout<<"department_id,number_of_orders,number_of_first_orders,percentage"<<endl;
    }
    catch( const char* cstr )
    {
        std::cerr << cstr << '\n' ;
    }
    for (auto i : p_dept_orders_map){     
        m_percentage = ((float)i.second.second/(float)i.second.first);
        p_fout<<i.first<<","<<i.second.first<<","<<i.second.second<<","<<std::fixed<<setprecision(2)<<m_percentage<<endl;
    }
}

int main(){
    fstream fin;
    fstream fout;
    unordered_map<int,int> prod_dept_map;
    unordered_map<string,int> header_index_map;
    map<int,pair<int,int>> dept_orders_map;
    vector<string> data_items;
    string l_infilepath1,l_infilepath2,l_outfilepath,row,col;
    int l_headerlinecount = 0;
    l_infilepath1 = "./input/products.csv";
    l_infilepath2 = "./input/order_products.csv";
    l_outfilepath = "./output/report.csv";
    const string C_PRODUCT = "product_id";
    const string C_DEPARTMENT = "department_id";
    const string C_REORDERED = "reordered";

    
    readFile(l_infilepath1, fin);
    while(fin){
        data_items = {};
        if(l_headerlinecount==0){
            headerMap(fin,header_index_map,l_headerlinecount);
        }
        else{
            data_items = splitLine(fin);
            if(!data_items.empty()&& data_items.size()==header_index_map.size()){
                prod_dept_map[stoi(data_items[header_index_map[C_PRODUCT]])] = stoi(data_items[header_index_map[C_DEPARTMENT]]);
            }
        }
    }
    fin.close(); 


    l_headerlinecount=0;
    header_index_map = {};
    readFile(l_infilepath2, fin);
    while(fin){
        data_items = {};
        if(l_headerlinecount==0){
            headerMap(fin,header_index_map,l_headerlinecount);
        }
        else{
            data_items = splitLine(fin);
            if(!data_items.empty() && data_items.size()==header_index_map.size()){
                if(prod_dept_map.find(stoi(data_items[header_index_map[C_PRODUCT]]))!=prod_dept_map.end()){
                dept_orders_map[prod_dept_map[stoi(data_items[header_index_map[C_PRODUCT]])]].first++;
                if(stoi(data_items[header_index_map[C_REORDERED]])==0){
                    dept_orders_map[prod_dept_map[stoi(data_items[header_index_map[C_PRODUCT]])]].second++;
                }  
            }
            }
        }
    }
    fin.close();

    writeFile(l_outfilepath,fout,dept_orders_map);
    fout.close();

    return 0;
}