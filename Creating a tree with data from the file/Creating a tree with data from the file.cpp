#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

struct Node {
    int code;
    string city;
    struct Node* left;
    struct Node* right;
    Node(int code, string city_name) : code(code), city(city_name), left(nullptr), right(nullptr) {}
};

class Tree {
public:
    Node* root = nullptr;

    void addNode(Node*& root, int code, string city_name) {
        if (root == nullptr) {
            root = new Node(code, city_name);
        }
        if (code < root->code) {
            addNode(root->left, code, city_name);
        }
        else if (code > root->code) {
            addNode(root->right, code, city_name);
        }
    }

    void preOrder(struct Node* root) {
        if (root == nullptr)
            return;
        cout << root->code << " ";
        preOrder(root->left);
        preOrder(root->right);
    }

    void inOrder(Node* root) {
        if (root == nullptr)
            return;
        inOrder(root->left);
        cout << root->code << " ";
        inOrder(root->right);
    }

    void postOrder(struct Node* root) {
        if (root == nullptr)
            return;
        postOrder(root->left);
        postOrder(root->right);
        cout << root->code << " ";
    }

    int calculateDepth(Node* root) {
        if (root == nullptr)
            return 0;
        int left = calculateDepth(root->left);
        int right = calculateDepth(root->right);
        return 1 + max(left, right);
    }

    bool checkBalance(Node* root) {
        if (root == nullptr)
            return false;
        int left = calculateDepth(root->left);
        int right = calculateDepth(root->right);
        if (abs(left - right) > 1)
            return false;
        return checkBalance(root->left) && checkBalance(root->right);
    }

    Node* searchByCode(Node* root, int code) {
        if (root == nullptr || root->code == code)
            return root;
        if (code < root->code)
            return searchByCode(root->left, code);
        else
            return searchByCode(root->right, code);
    }

    void printCityName(Node* root, int code) {
        Node* node = searchByCode(root, code);
        if (node == nullptr)
            cout << "Plaka kodu bulunamadi." << endl;
        else
            cout << "Sehir: " << node->city << endl;
    }

    int calculateLevelDifference(Node* root, int code1, int code2) {
        Node* node1 = searchByCode(root, code1);
        Node* node2 = searchByCode(root, code2);
        if (node1 == nullptr || node2 == nullptr)
            return -1;
        int level1 = 0, level2 = 0;
        Node* current_node = root;
        while (current_node != nullptr && current_node->code != code1) {
            if (code1 < current_node->code) {
                current_node = current_node->left;
                level1++;
            }
            else {
                current_node = current_node->right;
                level1++;
            }
        }
        current_node = root;
        while (current_node != nullptr && current_node->code != code2) {
            if (code2 < current_node->code) {
                current_node = current_node->left;
                level2++;
            }
            else {
                current_node = current_node->right;
                level2++;
            }
        }
        return abs(level1 - level2);
    }

    void printTree(Node* root, int level) {
        if (root == nullptr)
            return;
        printTree(root->right, level + 1);
        for (int i = 0; i < level; i++) {
            cout << "    ";
        }
        if (level > 0)
            cout << "|-";
        cout << root->code << "  " << endl;
        printTree(root->left, level + 1);
    }
};

int main()
{
    ifstream file("city_codes.txt");
    string line = "";
    string codes[18][18];
    int row1 = 0;
    int row2 = 0;

    Tree tree;

    if (file.is_open())
    {
        while (getline(file, line) && row1 < 18)
        {
            if (line.length() >= 2)
            {
                codes[row1][0] = line.substr(0, 2);
                codes[row1][1] = line.substr(3, 9);
                row1++;
            }
        }
        file.close();
    }

    for (int i = 0; i < 18; i++)
    {
        tree.addNode(tree.root, stoi(codes[i][0]), codes[i][1]);
    }

    cout << "Depth: " << tree.calculateDepth(tree.root) << ", ";

    if (tree.checkBalance(tree.root))
        cout << "Balanced" << endl;
    else
        cout << "Unbalanced" << endl;

    tree.printTree(tree.root, 0);

    int code1 = 0, code2 = 0;
    cout << "Enter two city codes to calculate the level difference: ";
    cin >> code1 >> code2;
    tree.printCityName(tree.root, code1);
    tree.printCityName(tree.root, code2);
    int difference = tree.calculateLevelDifference(tree.root, code1, code2);
    if (difference == -1)
        cout << "City codes not found." << endl;
    else
        cout << "Level difference in the tree: " << difference << endl;

    cout << "Preorder: ";
    tree.preOrder(tree.root);
    cout << endl;

    cout << "Inorder: ";
    tree.inOrder(tree.root);
    cout << endl;

    cout << "Postorder: ";
    tree.postOrder(tree.root);
    cout << endl;

    return 0;
}
