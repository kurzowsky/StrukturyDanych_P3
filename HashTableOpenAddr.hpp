#ifndef HASHTABLE_OPEN_ADDR_HPP
#define HASHTABLE_OPEN_ADDR_HPP

class hashTableOpenAddr {
private:
    // stany kubelkow
    enum bucketState { EMPTY, OCCUPIED, DELETED };

    struct openAddrEntry {
        int key;
        int value;
        bucketState state;

        openAddrEntry() : key(0), value(0), state(EMPTY) {}
    };

    openAddrEntry* oa_table;
    int max_size;
    int num_items;

    // funkncja mieszajaca (zabezpieczenie przed ujemnymi kluczami)
    int calcHash(int key) {
        return (key % max_size + max_size) % max_size;
    }

public:
    hashTableOpenAddr(int cap = 100) {
        max_size = cap;
        num_items = 0;
        oa_table = new openAddrEntry[max_size];
    }

    ~hashTableOpenAddr() {
        delete[] oa_table;
    }

    void insert(int key, int value) {
        // blokada przed przepelnieniem tablicy
        if (num_items >= max_size) {
            return;
        }

        int hashIndex = calcHash(key);
        int i = 0;

        // linear probing - szukamy wolnego miejsca lub istniej¹cego klucza do nadpisania
        while (oa_table[hashIndex].state == OCCUPIED && oa_table[hashIndex].key != key) {
            i++;
            hashIndex = (calcHash(key) + i) % max_size;
        }

        if (oa_table[hashIndex].state != OCCUPIED) {
            num_items++;
        }

        oa_table[hashIndex].key = key;
        oa_table[hashIndex].value = value;
        oa_table[hashIndex].state = OCCUPIED;
    }

    bool remove(int key) {
        int hashIndex = calcHash(key);
        int i = 0;

        // szukamy, pusty kubelek = koniec klastra
        while (oa_table[hashIndex].state != EMPTY) {
            if (oa_table[hashIndex].state == OCCUPIED && oa_table[hashIndex].key == key) {
                oa_table[hashIndex].state = DELETED; // zmiana stanu zamiast usuwac fizycznie
                num_items--;
                return true;
            }
            i++;
            hashIndex = (calcHash(key) + i) % max_size;

            // blokada przed zapetleniem
            if (i >= max_size) {
                break;
            }
        }
        return false;
    }

    int getSize() {
        return num_items;
    }
};

#endif 
