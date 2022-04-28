#pragma once
#include <vector>
using namespace std;


void add_edge(vector<int> adj[], int src, int dest);
void printShortestDistance(vector<int> adj[], int s,int dest, int v);
void parser_edge(string edge_list, int vertices_number);