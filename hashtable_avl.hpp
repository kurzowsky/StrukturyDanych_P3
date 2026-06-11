#ifndef HASHTABLE_AVL_HPP
#define HASHTABLE_AVL_HPP

#include "avltree.hpp"
#include <iostream>

class HashTableAVL {
private:
    AVLNode** table; // Tablica wskaŸników na korzenie drzew AVL
	int capacity;    // pojemnoœæ tablicy (iloœæ kube³ków)
	int size;        // ilosc elementów 
    AVLTree avl;     

	// funkcja haszuj¹ca
    int hashFunction(int key) {
        int hash = key % capacity;
        if (hash < 0) {
            hash += capacity;
        }
        return hash;
    }

public:
	// konstruktor
    HashTableAVL(int cap = 11) {
        capacity = cap;
        size = 0;
        table = new AVLNode * [capacity];

		// wszystkie kube³ki puste
        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }
    }

	// destruktor
    ~HashTableAVL() {
        for (int i = 0; i < capacity; i++) {
            avl.destroyTree(table[i]);
        }
        delete[] table;
    }

	// wstawienie lub nadpisanie pary klucz-wartoœæ
    void insert(int key, int value) {
        int index = hashFunction(key);

		// sprawdzamy czy klucz ju¿ istnieje w drzewie AVL pod tym indeksem
        if (avl.search(table[index], key) == nullptr) {
			size++; // gdy klucz nie istnieje, to dodajemy nowy element, wiêc zwiêkszamy rozmiar s³ownika
        }

        // Wstawiamy/nadpisujemy element w drzewie AVL znajduj¹cym siê pod tym indeksem
        table[index] = avl.insert(table[index], key, value);
    }

	// usuniêcie pary o danym kluczu
    void remove(int key) {
        int index = hashFunction(key);

		// czy istnieje klucz
        if (avl.search(table[index], key) != nullptr) {
            table[index] = avl.remove(table[index], key);
			size--; // redukujemy rozmiar s³ownika
        }
    }

    // size
    int getSize() {
        return size;
    }
};

#endif // HASHTABLE_AVL_HPP