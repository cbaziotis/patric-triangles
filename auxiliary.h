#ifndef TRIANGLES_AUXILIARY_H
#define TRIANGLES_AUXILIARY_H

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <string>
#include "constants.h"


using namespace std;


typedef struct {
    int a;
    int b;
} Edge;

typedef struct {
    int id;
    vector<int> neighbors;
} Node;


int *seriealizeNode(Node node);

Node deseriealizeNode(int buffer[], int size);

vector<string> split(string str, const char *delimiter);

int node_proc_owner(int boundaries[], int size, int node);

long get_file_size(string fileName);

Edge parse_line(string line, const char *delimiter);

map<int, Node> parse_file(string filename, const char *delimiter);

int *partition_file(string filename, const char *delimiter, int chunks);

int intersectionCount(vector<int> a, vector<int> b);

int surrogateCount(map<int, Node> nodes, Node node);

#endif //TRIANGLES_AUXILIARY_H