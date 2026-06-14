#include <iostream>
#include <chrono>
#include <random>
#include <fstream>
#include <unordered_set>
#include "hashtable_avl.hpp"
#include "avltree.hpp"
#include "HashTableChain.hpp"
#include "HashTableOpenAddr.hpp"



using namespace std;
using namespace std::chrono;

// Generator losowych liczb
int getRandomInt(int min, int max) {
	int seed = random_device{}(); // Generowanie losowego ziarna
    mt19937 gen(seed);
    uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

void runPerformanceTests() {
    int sizes[] = { 10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000 };
    int numSizes = 10;
    int attempts = 100;

    ofstream file("wyniki_czasowe.csv");
    file << "Struktura,Rozmiar,Operacja,Czas[ns]\n";

    for (int i = 0; i < numSizes; i++) {
        int currentSize = sizes[i];
        cout << "\nTestowanie dla rozmiaru: " << currentSize << "\n";
        cout << "\nStruktura,Rozmiar,Operacja,Czas[ns]\n";

        int maxkey = currentSize * 10;

        int* value_testData = new int[currentSize]();
        int* key_testData = new int[currentSize]();
        int* key_testData_shuffle = new int[currentSize]();
        unordered_set<int> uniqueKeys;
        while (uniqueKeys.size() < currentSize) {
            uniqueKeys.insert(getRandomInt(currentSize, maxkey));
        }
        int j = 0;
        for (int key : uniqueKeys) {
            key_testData[j] = key;
			key_testData_shuffle[j] = key;
            value_testData[j] = getRandomInt(1, 100000);
            j++;
        }
		random_shuffle(key_testData_shuffle, key_testData_shuffle + currentSize);
        int* keyToRemove = new int[attempts];
        int* valueToInsert = new int[attempts];
        int* keyToInsert = new int[attempts];
        for (int j = 0; j < attempts; j++) {
            keyToRemove[j] = key_testData_shuffle[j];
            valueToInsert[j] = getRandomInt(0, 100000);
            keyToInsert[j] = getRandomInt(currentSize, maxkey);
        }

		// Testy: HASH TABLE AVL
        HashTableAVL AVL(currentSize);

        // Budowanie struktury 
        for (int j = 0; j < currentSize; j++) {
            AVL.insert(key_testData[j], value_testData[j]);
        }

        cout << "Poczatkowy rozmiar HashTableAVL: " << AVL.getSize() << "\n";

        // 1. insert
        auto start = high_resolution_clock::now();
        for (int j = 0; j < attempts; j++) {
            AVL.insert(keyToInsert[j], valueToInsert[j]);
        }
        auto end = high_resolution_clock::now();
        long long duration = duration_cast<nanoseconds>(end - start).count() / attempts;
        file << "HashTableAVL," << currentSize << ",insert," << duration << "\n";
        cout << "HashTableAVL," << currentSize << ",insert," << duration << "\n";

        // 2. remove
		start = high_resolution_clock::now();
        for (int j = 0; j < attempts; j++) {
            AVL.remove(keyToRemove[j]);
        }
		end = high_resolution_clock::now();
		duration = duration_cast<nanoseconds>(end - start).count() / attempts;
		file << "HashTableAVL," << currentSize << ",remove," << duration << "\n";
		cout << "HashTableAVL," << currentSize << ",remove," << duration << "\n";


		// Testy: HASH TABLE CHAIN
        hashTableChain chain(currentSize);

        // Budowanie struktury 
        for (int j = 0; j < currentSize; j++) {
            chain.insert(key_testData[j], value_testData[j]);
        }

        cout << "Poczatkowy rozmiar HashTableChain: " << chain.getSize() << "\n";

        // 1. insert
        start = high_resolution_clock::now();
        for (int j = 0; j < attempts; j++) {
            chain.insert(keyToInsert[j], valueToInsert[j]);
        }
        end = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(end - start).count() / attempts;
        file << "HashTableChain," << currentSize << ",insert," << duration << "\n";
        cout << "HashTableChain," << currentSize << ",insert," << duration << "\n";

        // 2. remove
        start = high_resolution_clock::now();
        for (int j = 0; j < attempts; j++) {
            chain.remove(keyToRemove[j]);
        }
        end = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(end - start).count() / attempts;
        file << "HashTableChain," << currentSize << ",remove," << duration << "\n";
        cout << "HashTableChain," << currentSize << ",remove," << duration << "\n";

		// Testy: HASH TABLE OPEN ADDRESSING
        hashTableOpenAddr openAddressing(currentSize);

        // Budowanie struktury 
        for (int j = 0; j < currentSize; j++) {
            openAddressing.insert(key_testData[j], value_testData[j]);
        }

        cout << "Poczatkowy rozmiar HashTableOpenAddressing: " << openAddressing.getSize() << "\n";

        // 1. insert
        start = high_resolution_clock::now();
        for (int j = 0; j < attempts; j++) {
            openAddressing.insert(keyToInsert[j], valueToInsert[j]);
        }
        end = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(end - start).count() / attempts;
        file << "HashTableOpenAddressing," << currentSize << ",insert," << duration << "\n";
        cout << "HashTableOpenAddressing," << currentSize << ",insert," << duration << "\n";

        // 2. remove
        start = high_resolution_clock::now();
        for (int j = 0; j < attempts; j++) {
            openAddressing.remove(keyToRemove[j]);
        }
        end = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(end - start).count() / attempts;
        file << "HashTableOpenAddressing," << currentSize << ",remove," << duration << "\n";
        cout << "HashTableOpenAddressing," << currentSize << ",remove," << duration << "\n";

        // Czyszczenie pamieci po danej iteracji
        delete[] value_testData;
        delete[] key_testData;
        delete[] key_testData_shuffle;
        delete[] keyToRemove;
        delete[] valueToInsert;
        delete[] keyToInsert;
    }
    file.close();
    cout << "Zakonczono! Wyniki w pliku wyniki_czasowe.csv\n";
}

void structureMenu(int choice) {
    HashTableAVL AVL_;
	hashTableChain chain_;
	hashTableOpenAddr openAddressing_;

    int subChoice = -1;
    int e, p;

    while (subChoice != 0) {
        cout << "\n--- MENU STRUKTURY ---\n";
        if (choice == 1) {
            cout << "--- HASH TABLE AVL ---\n";
        }
        else if (choice == 2) {
            cout << "--- HASH TABLE CHAIN ---\n";
        }
        else if (choice == 3) {
            cout << "--- HASH TABLE OPEN ADDRESSING ---\n";
        }

        cout << "1. Dodanie wartosci 'e' o kluczu 'p' [insert(e, p)]\n";
        cout << "2. Usuniecie pary o kluczu 'p' [remove(p)]\n";
        cout << "0. Powrot\n";
        cout << "Wybierz operacje: ";
        cin >> subChoice;

        if (choice == 1) { // AVL
            switch (subChoice) {
            case 1:
                cout << "Wartosc: "; cin >> e;
                cout << "Klucz: "; cin >> p;
                AVL_.insert(p, e);
                break;
            case 2:
                cout << "Klucz: "; cin >> p;
                if (AVL_.remove(p)) {
                    cout << "Usuniety element o kluczu: " << p << "\n";
                }
                else {
                    cout << "Nie znaleziono elementu o kluczu: " << p << "\n";
                }
                break;
            }
        }
        else if (choice == 2) { // Chain
            switch (subChoice) {
            case 1:
                cout << "Wartosc: "; cin >> e;
                cout << "Klucz: "; cin >> p;
                chain_.insert(p, e);
                break;
            case 2:
                cout << "Klucz: "; cin >> p;
                if (chain_.remove(p)) {
                    cout << "Usuniety element o kluczu: " << p << "\n";
                }
                else {
                    cout << "Nie znaleziono elementu o kluczu: " << p << "\n";
                }
                break;
            }
        }
        else if (choice == 3) { // Open Addressing
            switch (subChoice) {
            case 1:
                cout << "Wartosc: "; cin >> e;
                cout << "Klucz: "; cin >> p;
                openAddressing_.insert(p, e);
                break;
            case 2:
                cout << "Klucz: "; cin >> p;
                if (openAddressing_.remove(p)) {
                    cout << "Usuniety element o kluczu: " << p << "\n";
                }
                else {
                    cout << "Nie znaleziono elementu o kluczu: " << p << "\n";
                }
                break;
            }
		}
    }
}

int main() {
    int mainChoice = -1;

    while (mainChoice != 0) {
        cout << "\n=== MENU GLOWNE ===\n";
        cout << "1. Testuj Tablice Mieszajaca z AVL (Recznie)\n";
        cout << "2. Testuj Tablice Mieszajaca z Chain (Recznie)\n";
        cout << "3. Testuj Tablice Mieszajaca z Open Addressing (Recznie)\n";
        cout << "4. Uruchom automatyczne pomiary czasowe (Zapis do CSV)\n";
        cout << "0. Wyjdz\n";
        cout << "Wybierz opcje: ";
        cin >> mainChoice;

        switch (mainChoice) {
        case 1:
            structureMenu(mainChoice);
            break;
        case 2:
            structureMenu(mainChoice);
            break;
        case 3:
            structureMenu(mainChoice);
            break;
        case 4:
            runPerformanceTests();
            break;
        case 0:
            cout << "Zamykanie programu...\n";
            break;
        default:
            cout << "Nieznana opcja.\n";
        }
    }
    return 0;
}