#include "auxiliary.h"

/**
 * Converts (Serializes) the Node struct to a buffer of integers,
 * so i can be send over MPI to another processor
 */
int *seriealizeNode(Node node) {
    //allocate buffer array
    int *s = new int[node.neighbors.size() + 1];
    //set the first element = Node.id
    s[0] = node.id;
    //set the rest elements to be the vector elements
    for (int i = 0; i < node.neighbors.size(); ++i) {
        s[i + 1] = node.neighbors[i];
    }
    return s;
}


/**
 * Converts (Deserializes) the buffer of integers back to a Node struct
 */
Node deseriealizeNode(int buffer[], int size) {
    Node node;
    //get the Node.id
    node.id = buffer[0];
    //get the vector elements
    node.neighbors.reserve((unsigned long) (size - 1));
    for (int i = 1; i < size; ++i) {
        node.neighbors.push_back(buffer[i]);
    }
    return node;
}

vector<string> split(string str, const char *delimiter) {

    vector<string> parts;
    stringstream ss(str); // Turn the string into a stream.
    string tok;

    while (getline(ss, tok, *delimiter)) {
        parts.push_back(tok);
    }

    return parts;
}

long get_file_size(string fileName) {
    ifstream file(fileName, ios::binary | ios::ate);
    long size = file.tellg();
    file.close();
    return size;
}

/**
 * parses a line and returns the edge
 */
Edge parse_line(string line, const char *delimeter) {
    vector<string> sep = split(line, delimeter);
    Edge edge;
    edge.a = std::stoi(sep[0]);
    edge.b = std::stoi(sep[1]);
    return edge;
}

/**
 * returns the number of elements that exist on both vectors
 */
int intersectionCount(vector<int> a, vector<int> b) {
    int sum = 0;
    for (int i = 0; i < a.size(); ++i) {
        for (int j = 0; j < b.size(); ++j) {
            if (a[i] == b[j]) sum++;
        }
    }
    return sum;
}

/**
 * parses the a file with adjacency lists
 * and returns the nodes it contains in a map
 */
map<int, Node> parse_file(string filename, const char *delimiter) {
    string line;

    ifstream file(filename);
    map<int, Node> nodes;
    file.seekg(0, file.beg);

    if (file.is_open()) {
        while (getline(file, line) && line.at(0) != '#') {
            if (line.at(0) != '#') { // don't read comments
                Edge edge = parse_line(line, delimiter);
                nodes[edge.a].id = edge.a;
                nodes[edge.a].neighbors.push_back(edge.b);
            }
        }
        file.close();
        remove(filename.c_str());
    }
    else {
        cout << "Unable to open file";
    }

    return nodes;
}

/**
 * The input file is divided into P chunks.
 * If a chunk boundary falls in the middle of an adjacency list,
 * the boundary is shifted so that the entire adjacency list
 * is in only one chunk.
 */
int *partition_file(string filename, const char *delimiter, int chunks) {

    string line;
    ifstream file(filename);
    long file_size = get_file_size(filename);
    long chunk_size = file_size / chunks;
    long split_boundary = chunk_size;
    bool needsSplit = false;
    int *boundaries = new int[chunks];
    Edge last, curr_node;
    int current = 0;

    ofstream temp_file;
    temp_file.open(CHUNK_PREFIX + to_string(current) + ".txt");
    cout << "\t-splitting chunk " << current << "..." << endl;
    if (file.is_open()) {
        while (getline(file, line)) {

            if (line.at(0) != '#') { // don't read comments

                curr_node = parse_line(line, delimiter);

                if (needsSplit) {

                    //check if we are in the middle of an adjacency list
                    if (last.a != curr_node.a) {
                        //close the current open file
                        temp_file.close();
                        boundaries[current] = last.a;

                        current++;
                        needsSplit = false;

                        //open a new file to right the next chunk
                        temp_file.open(CHUNK_PREFIX + to_string(current) + ".txt");
                        cout << "\t-splitting chunk " << current << "..." << endl;
                    }

                } else {

                    //check if the file pointer has reached the next boundary
                    //if it has we can't split right now, we have to check
                    //if we are in the middle of an adjacency list
                    if (file.tellg() > split_boundary) {
                        last = parse_line(line, delimiter);
                        needsSplit = true;
                        split_boundary += chunk_size;
                    }

                }

                temp_file << line << endl;
            }
        }

        file.close();
        temp_file.close();
        boundaries[current] = curr_node.a;
    }
    else {
        cout << "Unable to open file";
    }

    return boundaries;

}

int node_proc_owner(int boundaries[], int size, int node) {
    for (int i = 0; i < size; i++) {
        if (node <= boundaries[i]) return i;
    }
    return 0;
}

/**
 * The function for computing the surrogateCount
 * that is described in the paper's algorithm (Fig. 2)
 */
int surrogateCount(map<int, Node> nodes, Node node) {

    int sum = 0;

    for (int i = 0; i < node.neighbors.size(); ++i) {

        int id = node.neighbors[i];

        if (nodes.find(id) != nodes.end()) {
            sum += intersectionCount(nodes[id].neighbors, node.neighbors);
        }

    }

    return sum;
}
