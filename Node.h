#include <vector>
using namespace std;

struct Node{
    int color;
    int height;
    Node* children[4];
    Node() {}
};