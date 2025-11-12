#pragma once
#include<iostream>
#include<unordered_map>
#include<vector>
#include<string>
#include <fstream>
#include <sstream>
using namespace std;

class Itemset {
public:
    int id;
    string item;
    vector<int>ids;
    int support;
};

class Transaction {
public:
	unordered_map<int, bool> items;
};

class Apriori
{
public:
    int minSupport;
    int minConfidence;
    int id = 0;

    vector<Transaction> Transactions;
    vector<vector<Itemset>> CandidateItemsets; // candidate of level and vector of itemsets
    vector<vector<Itemset>> FrequentItemsets;

    unordered_map<string, int> itemIDMap;
    unordered_map<int, string> idItemMap;

    void Calc_support(vector<int>);
    void Join_step(int);
    void Setup(string filepath);
};
