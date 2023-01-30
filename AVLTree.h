//
// Created by giova on 4/10/2022.
//

#ifndef INC_22S_FINAL_PROJ_AVLTREE_H
#define INC_22S_FINAL_PROJ_AVLTREE_H

#include <algorithm>

using namespace std;

template<typename T>
class AVLTree {
private:

    // Node struct for the AVL Tree
    template<typename U>
    struct AVLNode {
        U data;
        AVLNode<U>* left;
        AVLNode<U>* right;
        int height;

        // Default constructor
        AVLNode() {
            this->left = nullptr;
            this->right = nullptr;
            this->height = 0;
        }

        // Parameter constructor
        AVLNode(const U& data, AVLNode<U>* l = nullptr, AVLNode<U>* r = nullptr, int h = 0) {
            this->data = data;
            this->left = l;
            this->right = r;
            this->height = h;
        }
    };

    AVLNode<T>* root;

    int size;

    /**
     * Inserts a new element in the tree.
     * @return Reference to a data type.
     **/
    T& insert(const T& x, AVLNode<T>*& t);

    /**
     * Balances the tree.
     * @return Void.
     **/
    void balance(AVLNode<T>*& t);

    /**
     * Performs a rotation when an element is inserted into the left
     * subtree of the left child of alpha (k2).
     * @return Void.
     **/
    void rotateWithLeftChild(AVLNode<T>*& k2);
    /**
     * Performs a rotation when an element is inserted into the right
     * subtree of the left child of alpha (k2).
     * @return Void.
     **/
    void doubleWithLeftChild(AVLNode<T>*& k2);
    /**
     * Performs a rotation when an element is inserted into the right
     * subtree of the right child of alpha (k2).
     * @return None.
     **/
    void rotateWithRightChild(AVLNode<T>*& k2);
    /**
     * Performs a rotation when an element is inserted into the left
     * subtree of the right child of alpha (k2).
     * @return None.
     **/
    void doubleWithRightChild(AVLNode<T>*& k2);

    /**
     * Calculates and returns the height of a node (t).
     * @return Integer.
     **/
    int height(AVLNode<T>* t);

    /**
     * Returns a boolean variable indicating if an element is present
     * in the tree.
     * @return Boolean.
     **/
    bool contains(AVLNode<T>* c, const T& arg);

    /**
     * Clears the tree.
     * @return Void.
     **/
    void clear(AVLNode<T>* r);

    /**
     * Clones a tree.
     * @return Pointer to an AVL Node.
     **/
    AVLNode<T>* clone(AVLNode<T>* t) const {
        // If node is null pointer
        if(t == nullptr) {
            // Return null pointer
            return nullptr;
        }
        else {
            // Recursively clone all nodes in the tree
            return new AVLNode<T>(t->data, clone(t->left), clone(t->right), t->height);
        }
    }

    /**
     * Searches for an element in the tree and returns a reference to
     * that element. Note: Contains have to be performed before
     * calling this function.
     * @return Reference to a data type.
     **/
    T& search(AVLNode<T>* c, const T& arg);

    /**
     * Prints the tree to the console.
     * @return Void.
     **/
    void print(AVLNode<T>* r);

    /**
     * Recursively adds elements to generate a list from the tree.
     * @return None.
     **/
    void addToList(AVLNode<T>* r, T* data, int& i);

public:

    /**
     * Default constructor.
     * @return None.
     **/
    AVLTree() : root(nullptr), size(0) {}

    /**
     * Copy constructor.
     * @return None.
     **/
    AVLTree(const AVLTree<T>& arg) : root(nullptr) {
        // Clone tree
        root = clone(arg.root);
        // Set size to argument size
        size = arg.size;
    }

    /**
     * Assignment operator.
     * @return None.
     **/
    AVLTree<T>& operator=(const AVLTree<T>& arg) {
        // Clone tree
        root = clone(arg.root);
        // Set size to argument size
        size = arg.size;
        // Return this tree
        return *this;
    }

    /**
     * Destructor.
     * @return None.
     **/
    ~AVLTree() {
        clear(root);
        size = 0;
    }

    /**
     * Recursively insert an element into the tree. Returns the
     * element inserted or an existent element if found.
     * @return Reference to a data type.
     **/
    T& insert(const T& x) {
        // Recursively call insert and return the element
        return insert(x, root);
    }

    /**
     * Recursively checks if an element is in the tree.
     * @return Boolean.
     **/
    bool contains(const T& arg) {
        // Recursively check for the element in the tree
        return contains(root, arg);
    }

    /**
     * Recursively clears the tree.
     * @return Void.
     **/
    void clear() {
        // Recursively clear the tree
        clear(root);
        // Set root to null pointer
        root = nullptr;
        // Set size to 0
        size = 0;
    }

    /**
     * Recursively searches for an element in the tree.
     * Note: Contains have to be performed before calling this
     * function.
     * @return Reference to a data type.
     **/
    T& search(const T& arg) {
        // Recursively search for an element in the tree
        return search(root, arg);
    }

    /**
     * Recursively prints the tree to the console.
     * @return Void.
     **/
    void print() {
        // Recursively print the tree
        print(root);
    }

    /**
     * Returns the size of the tree.
     * @return None.
     **/
    int getSize();

    /**
     * Returns an array containing all the elements of the tree in
     * ascending order. Note: Array needs to be deleted after use to
     * avoid memory leaks.
     * @return Pointer to a data type.
     **/
    T* getList();
};

template<typename T>
T& AVLTree<T>::insert(const T& x, AVLNode<T>*& t){
    // If node is null pointer
    if (t == nullptr) {
        // Insert a new node at that location
        t = new AVLNode<T>(x);
        // Increment size
        size++;
    }
    // If element is already in the tree
    else if (x == t->data) {
        // Return that element
        return t->data;
    }
    // If element is less than the data in the node
    else if (x < t->data) {
        // Recursively call insert on the left node
        return insert(x, t->left);
    }
    // If element is greater than the data in the node
    else if (x > t->data) {
        // Recursively call insert on the right node
        return insert(x, t->right);
    }
    // Balance tree (if necessary)
    balance(t);
    // Return data in node
    return t->data;
}

template<typename T>
void AVLTree<T>::balance(AVLNode<T>*& t) {
    // If node is null pointer, there is no need to balance
    if (t == nullptr) {
        return;
    }
    // If difference in height of the left node minus the right node
    // is greater than one, it can be LL or LR rotation
    if (height(t->left) - height(t->right) > 1) {
        // If the height of the left node of the left node is
        // greater or equal to the right node of the left node
        if (height(t->left->left) >= height(t->left->right)) {
            // Perform Case 1 rotation (LL)
            rotateWithLeftChild(t);
        }
        // Otherwise
        else {
            // Perform Case 2 rotation (LR)
            doubleWithLeftChild(t);
        }
    }
    // If difference in height of the right node minus the left node
    // is greater than one, it can be RR or RL rotation
    else if (height(t->right) - height(t->left) > 1) {
        // If the height of the right node of the right node is
        // greater or equal to the left node of the right node
        if (height(t->right->right) >= height(t->right->left)) {
            // Perform Case 4 rotation (RR)
            rotateWithRightChild(t);
        }
        // Otherwise
        else {
            // Perform Case 3 rotation (RL)
            doubleWithRightChild(t);
        }
    }
    // Set node height to the max between the left and the right node
    t->height = max(height(t->left), height(t->right)) + 1;
}

template<typename T>
void AVLTree<T>::rotateWithLeftChild(AVLNode<T>*& k2) {
    AVLNode<T>* k1 = k2->left;

    k2->left = k1->right;
    k1->right = k2;

    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), height(k1->right)) + 1;

    // Set new root
    k2 = k1;
}

template<typename T>
void AVLTree<T>::doubleWithLeftChild(AVLNode<T>*& k2) {
    // Perform LL rotation first
    rotateWithLeftChild(k2);

    AVLNode<T>* k1 = k2->right;

    k2->right = k1->left;
    k1->left = k2;

    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), height(k1->right)) + 1;

    // Set new root
    k2 = k1;
}

template<typename T>
void AVLTree<T>::rotateWithRightChild(AVLNode<T>*& k2) {
    AVLNode<T>* k1 = k2->right;

    k2->right = k1->left;
    k1->left = k2;

    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), height(k1->right)) + 1;

    // Set new root
    k2 = k1;
}

template<typename T>
void AVLTree<T>::doubleWithRightChild(AVLNode<T>*& k2) {
    // Perform RR rotation first
    rotateWithRightChild(k2);

    AVLNode<T>* k1 = k2->left;

    k2->left = k1->right;
    k1->right = k2;

    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), height(k1->right)) + 1;

    // Set new root
    k2 = k1;
}

template<typename T>
int AVLTree<T>::height(AVLNode<T>* t) {
    // Calculate and returns the height of the node
    return t == nullptr ? -1 : t->height;
}

template<typename T>
bool AVLTree<T>::contains(AVLNode<T>* c, const T& arg) {
    // If node is null pointer
    if (c == nullptr) {
        // Return false
        return false;
    }
    // If the data in the node is equal to the argument
    else if (c->data == arg) {
        // Return true
        return true;
    }
    // If the argument is less than the data in the node
    else if (arg < c->data) {
        // Recursively search on the left node
        return contains(c->left, arg);
    }
    // If the argument is greater than the data in the node
    else {
        // Recursively search on the right node
        return contains(c->right, arg);
    }
}

template<typename T>
void AVLTree<T>::clear(AVLNode<T>* r) {
    // Recursively clear tree in post order traversal
    if (r != nullptr) {
        clear(r->left);
        clear(r->right);
        r->left = nullptr;
        r->right = nullptr;
        delete r;
    }
}

template<typename T>
T& AVLTree<T>::search(AVLNode<T>* c, const T& arg) {
    // If the data in the node is equal to the argument
    if (c->data == arg) {
        // Return the data in the pointer
        return c->data;
    }
    // If the argument is less than the data in the node
    else if (arg < c->data) {
        // Recursively search on the left node
        return search(c->left, arg);
    }
    // If the argument is greater than the data in the node
    else {
        // Recursively search on the right node
        return search(c->right, arg);
    }
}

template<typename T>
void AVLTree<T>::print(AVLNode<T>* r) {
    // Recursively print tree using in order traversal
    if (r != nullptr) {
        print(r->left);
        cout << r->data << endl;
        print(r->right);
    }
}

template<typename T>
int AVLTree<T>::getSize() {
    // Return tree size
    return size;
}

template<typename T>
T* AVLTree<T>::getList() {
    // Allocate memory for the list
    T* data = new T[size];
    // Initialize list size to 0
    int i = 0;
    // Recursively add all the elements of the tree to the list
    addToList(root, data, i);
    // Return list
    return data;
}

template<typename T>
void AVLTree<T>::addToList(AVLNode<T>* r, T* data, int& i) {
    // Add elements to the list using in order traversal
    if (r != nullptr) {
        addToList(r->left, data, i);
        data[i] = r->data;
        i++;
        addToList(r->right, data, i);
    }
}

#endif //INC_22S_FINAL_PROJ_AVLTREE_H
