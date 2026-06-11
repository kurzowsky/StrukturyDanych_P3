#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include <iostream>

// wezel AVL
struct AVLNode {
    int key;
    int value;
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(int k, int v) {
        key = k;
        value = v;
        height = 1;
        left = nullptr;
        right = nullptr;
    }
};

class AVLTree {
public:
    // wysokosc wezla
    int getHeight(AVLNode* node) {
        if (node == nullptr) return 0;
        return node->height;
    }

	// balans wezla
    int getBalance(AVLNode* node) {
        if (node == nullptr) return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

	// maksium z dwóch liczb
    int max(int a, int b) {
        return (a > b) ? a : b;
    }

	// right rotation
    AVLNode* rotateRight(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

		// rotacja
        x->right = y;
        y->left = T2;

		// zmiana wysokosci
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }

	// left rotation
    AVLNode* rotateLeft(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

		// rotacja
        y->left = x;
        x->right = T2;

		// zmiana wysokosci
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }

	// wstawianie
    AVLNode* insert(AVLNode* node, int key, int value) {
        if (node == nullptr)
            return new AVLNode(key, value);

        if (key < node->key)
            node->left = insert(node->left, key, value);
        else if (key > node->key)
            node->right = insert(node->right, key, value);
        else {
            // jezeli klucz ju¿ istnieje nadpisujemy wartoœæ
            node->value = value;
            return node;
        }

		// zmiana wysokosci tego przodka
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

		// wspó³czynnik balansu tego przodka
        int balance = getBalance(node);

		// zmiana balansu i balansowanie drzewa

        // LL
        if (balance > 1 && key < node->left->key)
            return rotateRight(node);

        // RR
        if (balance < -1 && key > node->right->key)
            return rotateLeft(node);

        // LR
        if (balance > 1 && key > node->left->key) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // RL
        if (balance < -1 && key < node->right->key) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

	// weze³ o najmniejszym kluczu
    AVLNode* minValueNode(AVLNode* node) {
        AVLNode* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    // usuwanie
    AVLNode* remove(AVLNode* root, int key) {
        if (root == nullptr)
            return root;

        if (key < root->key)
            root->left = remove(root->left, key);
        else if (key > root->key)
            root->right = remove(root->right, key);
        else {
			// gdy znajdziemy wêze³ do usuniêcia
            if ((root->left == nullptr) || (root->right == nullptr)) {
				// przypadek jedno dziecko lub brak dzieci
                AVLNode* temp = root->left ? root->left : root->right;

				if (temp == nullptr) { // gdy brak dzieci
                    temp = root;
                    root = nullptr;
                }
				else { // jedno dziecko
					*root = *temp; // kopiujemy dane z dziecka do aktualnego wêz³a
                }
                delete temp;
            }
            else {
				// 2 dzieci: znajdŸ nastêpnika (najmniejszy w prawym poddrzewie)
                AVLNode* temp = minValueNode(root->right);

				// kopiujemy dane z nastêpnika do aktualnego wêz³a
                root->key = temp->key;
                root->value = temp->value;

				// usuwamy nastêpnika
                root->right = remove(root->right, temp->key);
            }
        }

        // gdy drzewo ma tylko jeden wêze³
        if (root == nullptr)
            return root;

		// zmiana wysokosci tego przodka
        root->height = 1 + max(getHeight(root->left), getHeight(root->right));

		// balansowanie tego przodka
        int balance = getBalance(root);

		// balansowanie drzewa po usuniêciu

        // LL
        if (balance > 1 && getBalance(root->left) >= 0)
            return rotateRight(root);

        // LR
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }

        // RR
        if (balance < -1 && getBalance(root->right) <= 0)
            return rotateLeft(root);

        // RL
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }

        return root;
    }

	// search
    AVLNode* search(AVLNode* root, int key) {
        if (root == nullptr || root->key == key)
            return root;

        if (root->key < key)
            return search(root->right, key);

        return search(root->left, key);
    }

    void destroyTree(AVLNode* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
};

#endif // AVLTREE_HPP