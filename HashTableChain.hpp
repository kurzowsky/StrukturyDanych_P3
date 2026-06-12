#ifndef HASHTABLECHAIN_HPP
#define HASHTABLECHAIN_HPP


struct chainNode {
    int key;
    int value;
    chainNode* next;

    chainNode(int k, int v) : key(k), value(v), next(nullptr) {}
};

class hashTableChain {
private:
    chainNode** chain_table;
    int max_capacity;
    int num_elements;

    // funkncja mieszajaca (zabezpieczenie przed ujemnymi kluczami)
    int compute_hash(int key) {
        return (key % max_capacity + max_capacity) % max_capacity;
    }

public:
    hashTableChain(int cap = 100) {
        max_capacity = cap;
        num_elements = 0;
        chain_table = new chainNode * [max_capacity];
        for (int i = 0; i < max_capacity; i++) {
            chain_table[i] = nullptr;
        }
    }

    ~hashTableChain() {
        for (int i = 0; i < max_capacity; i++) {
            chainNode* entry = chain_table[i];
            while (entry != nullptr) {
                chainNode* prev = entry;
                entry = entry->next;
                delete prev;
            }
        }
        delete[] chain_table;
    }


    void insert(int key, int value) {
        int hashIndex = compute_hash(key);
        chainNode* entry = chain_table[hashIndex];

        // sprzawdzenie czy klucz istnieje
        while (entry != nullptr) {
            if (entry->key == key) {
                entry->value = value;
                return;
            }
            entry = entry->next;
        }

        // nowy elem. na poczatku listy
        entry = new chainNode(key, value);
        entry->next = chain_table[hashIndex];
        chain_table[hashIndex] = entry;
        num_elements++;
    }

    
        bool remove(int key) {
        int hashIndex = compute_hash(key);
        chainNode* prev = nullptr;
        chainNode* entry = chain_table[hashIndex];

        while (entry != nullptr) {
            if (entry->key == key) {
                if (prev == nullptr) {
                    chain_table[hashIndex] = entry->next; // usuwanie pierwszego elementu 
                }
                else {
                    prev->next = entry->next;      // usuwanie z glebi listy
                }
                delete entry;
                num_elements--;
                return true;
            }
            prev = entry;
            entry = entry->next;
        }
        return false;
    }
};

#endif 