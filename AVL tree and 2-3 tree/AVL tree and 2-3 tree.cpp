#include <iostream>
#include <algorithm>
#include <queue>
#include <iomanip>
#include <string> // 包含 string 標頭檔案

using namespace std;

// AVL 樹節點
struct AVLNode { //struct 是用來定義結構體
    int key;
    AVLNode* left;
    AVLNode* right;//定義了 AVLNode 結構體中的兩個成員變數 left 和 right
    int height;
	AVLNode(int k) : key(k), left(nullptr), right(nullptr), height(1) {} //初始化列表
    //• AVLNode(int k)：這是建構函式的宣告，表示這個建構函式接受一個整數參數 k，用來初始化節點的鍵值。
    //• key(k)：這是初始化列表的一部分，表示將參數 k 的值賦給成員變數 key。
    //•	left(nullptr)：將成員變數 left 初始化為 nullptr，表示這個節點的左子節點一開始是空的。
    //•	right(nullptr)：將成員變數 right 初始化為 nullptr，表示這個節點的右子節點一開始是空的。
    //•	height(1)：將成員變數 height 初始化為 1，表示這個節點的高度一開始是 1。
};

// 獲取節點高度
int height(AVLNode* node) {
    return node ? node->height : 0;
}

// 獲取節點的平衡因子
int getBalance(AVLNode* node) {
	return node ? height(node->left) - height(node->right) : 0; 
    //如果 node 不是 nullptr，則返回左子樹的高度減去右子樹的高度，否則返回 0。
}

// 右旋轉
AVLNode* rightRotate(AVLNode* y) {
	AVLNode* x = y->left; //將x設為y的左子節點
	AVLNode* T2 = x->right; //將T2 設為 x 的右子節點
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

// 左旋轉
AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

// 插入節點
AVLNode* insert(AVLNode* node, int key) {
	if (!node) return new AVLNode(key); //如果節點為空，則創建一個新節點
    if (key < node->key)
		node->left = insert(node->left, key); //如果 key 小於節點的鍵值，則插入到左子樹
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key)
        return rightRotate(node);
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

// 找到具有最小鍵值的節點
AVLNode* minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

// 刪除節點
AVLNode* deleteNode(AVLNode* root, int key) {
    if (!root) return root;
    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        if (!root->left || !root->right) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (!temp) {
                temp = root;
                root = nullptr;
            }
            else
                *root = *temp;
            delete temp;
        }
        else {
            AVLNode* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }
    if (!root) return root;
    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

// 打印樹
void printTree(AVLNode* root, int space = 0, int height = 10) {
    if (!root) return;
    space += height;
    printTree(root->right, space);
    cout << endl;
    for (int i = height; i < space; i++)
        cout << ' ';
    cout << root->key << "\n";
    printTree(root->left, space);
}

// 2-3 樹節點
struct TwoThreeNode {
    int keys[2];
    TwoThreeNode* children[3];
    int numKeys;
    TwoThreeNode() : numKeys(0) {
        keys[0] = keys[1] = 0; // 初始化 keys 成員變數
        for (int i = 0; i < 3; ++i) children[i] = nullptr;
    }
};

// 插入 2-3 樹節點
TwoThreeNode* insert(TwoThreeNode* root, int key) {
    if (!root) {
        root = new TwoThreeNode();
        root->keys[0] = key;
        root->numKeys = 1;
        return root;
    }

    // 如果是葉子節點
    if (!root->children[0]) {
        if (root->numKeys == 1) {
            if (key < root->keys[0]) {
                root->keys[1] = root->keys[0];
                root->keys[0] = key;
            }
            else {
                root->keys[1] = key;
            }
            root->numKeys = 2;
        }
        else {
            // 分裂節點
            TwoThreeNode* newRoot = new TwoThreeNode();
            TwoThreeNode* newChild = new TwoThreeNode();
            if (key < root->keys[0]) {
                newRoot->keys[0] = root->keys[0];
                newRoot->children[0] = newChild;
                newRoot->children[1] = root;
                newChild->keys[0] = key;
            }
            else if (key < root->keys[1]) {
                newRoot->keys[0] = key;
                newRoot->children[0] = root;
                newRoot->children[1] = newChild;
                newChild->keys[0] = root->keys[1];
            }
            else {
                newRoot->keys[0] = root->keys[1];
                newRoot->children[0] = root;
                newRoot->children[1] = newChild;
                newChild->keys[0] = key;
            }
            root->numKeys = 1;
            root->keys[0] = root->keys[0];
            root->keys[1] = 0;
            root->children[1] = nullptr;
            return newRoot;
        }
    }
    else {
        // 遞歸插入到子節點
        if (key < root->keys[0]) {
            root->children[0] = insert(root->children[0], key);
        }
        else if (root->numKeys == 1 || key < root->keys[1]) {
            root->children[1] = insert(root->children[1], key);
        }
        else {
            root->children[2] = insert(root->children[2], key);
        }
    }
    return root;
}

// 打印 2-3 樹
void printTree(TwoThreeNode* root, int space = 0, int height = 10) {
    if (!root) return;
    space += height;
    if (root->numKeys == 2) printTree(root->children[2], space);
    cout << endl;
    for (int i = height; i < space; i++)
        cout << ' ';
    if (root->numKeys == 2) cout << root->keys[1] << " ";
    cout << root->keys[0] << "\n";
    printTree(root->children[0], space);
    if (root->numKeys == 2) printTree(root->children[1], space);
}

int main() {
    AVLNode* avlRoot = nullptr;
    TwoThreeNode* ttRoot = nullptr;
    int choice, value, treeType;
    while (true) {
        cout << "選擇樹型: 1. AVL 樹 2. 2-3 樹\n輸入你的選擇: ";
        cin >> treeType;
        if (treeType != 1 && treeType != 2) {
            cout << "無效的選擇\n";
            continue;
        }
        while (true) {
            cout << "1. 插入\n2. 刪除\n3. 打印\n輸入你的選擇 (輸入 'out' 返回): ";
            string subChoice;
            cin >> subChoice;
            if (subChoice == "out") break;
            choice = stoi(subChoice); // 使用 stoi 將字串轉換為整數
            switch (choice) {
            case 1:
                cout << "輸入要插入的值: ";
                cin >> value;
                if (treeType == 1)
                    avlRoot = insert(avlRoot, value);
                else
                    ttRoot = insert(ttRoot, value);
                break;
            case 2:
                cout << "輸入要刪除的值: ";
                cin >> value;
                if (treeType == 1)
                    avlRoot = deleteNode(avlRoot, value);
                else
                    cout << "2-3 樹刪除尚未實作\n"; // 需要實作 2-3 樹的刪除邏輯
                break;
            case 3:
                if (treeType == 1)
                    printTree(avlRoot);
                else
                    printTree(ttRoot);
                break;
            default:
                cout << "無效的選擇\n";
            }
        }
    }
}
