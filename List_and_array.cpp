#include <iostream>
#include <chrono>
using namespace std;

//Tablica dynamiczna
int* resize(int* tablica, int& capacity, int size) {
    capacity *= 2;
    int* temp = new int[capacity];
    for (int i = 0; i < size; i++) {
        temp[i] = tablica[i];
    }
    delete[] tablica;
    return temp;
}
void add(int*& tablica, int& size, int& capacity, int value, int index = -1) {
    if (size == capacity) {
        tablica = resize(tablica, capacity, size);
    }
    if (index == -1 || index >= size) {
        tablica[size++] = value;
    }
    else {
        for (int i = size; i > index; i--) {
            tablica[i] = tablica[i - 1];
        }
        tablica[index] = value;
        size++;
    }
}
void remove(int*& tablica, int& size, int index) {
    if (index < 0 || index >= size) {
        cout << "Indeks poza zakresem!" << endl;
        return;
    }
    for (int i = index; i < size - 1; i++) {
        tablica[i] = tablica[i + 1];
    }
    size--;
}
int find_value(int* tablica, int size, int value) {
    for (int i = 0; i < size; i++) {
        if (tablica[i] == value) {
            return i;
        }
    }
    return -1;
}
void print(int* tablica, int size) {
    for (int i = 0; i < size; i++) {
        cout << tablica[i] << " ";
    }
    cout << endl;
}
//Lista wiązana ze wskaźnikiem na ostatni węzeł
// Struktura przechowująca pojedynczy węzeł listy
struct Node {
    int value;
    Node* next;
    Node(int val) : value(val), next(nullptr) {}
};
// Klasa reprezentująca listę jednokierunkową ze wskaźnikiem na ostatni element (tail)
class LinkedList {
private:
    Node* head; // Wskaźnik na początek listy
    Node* tail; // Wskaźnik na koniec listy
    int size;   // Rozmiar listy

public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~LinkedList() {
        clear(); // Usuwa wszystkie węzły, aby uniknąć wycieków pamięci
    }
    // Dodawanie elementu na początek listy (O(1))
    void add_front(int value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
        }
        else {
            newNode->next = head;
            head = newNode;
        }
        size++;
    }
    // Dodawanie elementu na koniec listy (O(1))
    void add_back(int value) {
        Node* newNode = new Node(value);
        if (!tail) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }
    // Dodawanie elementu na określony indeks (O(n))
    void add_at(int value, int index) {
        if (index < 0 || index > size) {
            cout << "Indeks poza zakresem!" << endl;
            return;
        }
        if (index == 0) {
            add_front(value);
        }
        else if (index == size) {
            add_back(value);
        }
        else {
            Node* newNode = new Node(value);
            Node* current = head;
            for (int i = 0; i < index - 1; ++i) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
            size++;
        }
    }
    // Usuwanie elementu z początku listy (O(1))
    void remove_front() {
        if (!head) {
            cout << "Lista jest pusta!" << endl;
            return;
        }
        Node* temp = head;
        head = head->next;
        if (!head) {
            tail = nullptr; // Lista stała się pusta
        }
        delete temp;
        size--;
    }
    // Usuwanie elementu z końca listy (O(n))
    void remove_back() {
        if (!tail) {
            cout << "Lista jest pusta!" << endl;
            return;
        }
        if (head == tail) {
            delete head;
            head = tail = nullptr;
        }
        else {
            Node* current = head;
            while (current->next != tail) {
                current = current->next;
            }
            delete tail;
            tail = current;
            tail->next = nullptr;
        }
        size--;
    }
    // Usuwanie elementu z określonego indeksu (O(n))
    void remove_at(int index) {
        if (index < 0 || index >= size) {
            cout << "Indeks poza zakresem!" << endl;
            return;
        }
        if (index == 0) {
            remove_front();
        }
        else if (index == size - 1) {
            remove_back();
        }
        else {
            Node* current = head;
            for (int i = 0; i < index - 1; ++i) {
                current = current->next;
            }
            Node* temp = current->next;
            current->next = temp->next;
            delete temp;
            size--;
        }
    }
    // Wyszukiwanie elementu (O(n))
    int find(int value) {
        Node* current = head;
        int index = 0;
        while (current) {
            if (current->value == value) {
                return index;
            }
            current = current->next;
            index++;
        }
        return -1; // Nie znaleziono
    }
    // Wyświetlanie listy (O(n))
    void print() {
        Node* current = head;
        while (current) {
            cout << current->value << " ";
            current = current->next;
        }
        cout << endl;
    }
    // Usuwanie wszystkich elementów (O(n))
    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        size = 0;
    }

    // Zwraca rozmiar listy
    int get_size() const {
        return size;
    }
};
int main() {
    
    const int copies = 100;//Liczba kopi struktór (zrówno tablic dynamicznych jak i list)
    const int elements = 100000;//Liczba początkowych elementów w tych strukturach danych
    
    int** array_of_arrays = new int* [copies];
    int* array_sizes = new int[copies];
    int* array_capacities = new int[copies];

    LinkedList* array_of_lists = new LinkedList[copies];

    //Inicjalizacja tablicy dynamicznej o początkowym rozmiarze równym 1
    for (int i = 0; i < copies; i++) {
        array_of_arrays[i] = new int[1];
        array_sizes[i] = 0;
        array_capacities[i] = 1;
    }

    //Dodawanie określonej liczby początkowych elementów do każdej z kopi tablicy i listy (zmienna elements)
    for (int i = 0; i < copies; i++) {
        for (int j = 0; j < elements; j++) {
            add(array_of_arrays[i], array_sizes[i], array_capacities[i], j);
            array_of_lists[i].add_back(j);
        }
    }

    /// Test wydajności dla tablicy dynamicznej (należy wybrać odpowiednią funkcję)
    //int find_index = -1;//Można użyć tej zmiennej do przypisania indeksu, na którym znaleziono wybraną wartość
    auto start_arrays = chrono::high_resolution_clock::now();//Czas start
    for (int i = 0; i < copies; i++) {
        //add(array_of_arrays[i], array_sizes[i], array_capacities[i], 1, 0);//Dodawanie wartości np. 1 na początek
        //add(array_of_arrays[i], array_sizes[i], array_capacities[i], 1, 5);//Dodawanie wartości np. 1 na wybrany indeks np. 5
        //add(array_of_arrays[i], array_sizes[i], array_capacities[i], 1);//Dodawanie na koniec wartości 1
        //remove(array_of_arrays[i], array_sizes[i], 0); // Usuwanie z początku
        //remove(array_of_arrays[i], array_sizes[i], array_sizes[i]-1); // Usuwanie z końca
        //remove(array_of_arrays[i], array_sizes[i], 5);// Usuwanie z wybranego indeksu np. 5
        find_value(array_of_arrays[i], array_sizes[i], 0);//Wyszukiwanie zadanej wartości, która nie jest wyświetlana, bo wymaga przypisania do zmiennej, co dodatkowo zajmuje czas zaburzając wynik
        ///Przykładowy kod wypisujący indeks do zmiennej znajduje się poniżej:
        //find_index = find_value(array_of_arrays[i], array_sizes[i], 0);
    }
    auto end_arrays = chrono::high_resolution_clock::now();//Czas stop
    double array_time = chrono::duration<double>(end_arrays - start_arrays).count() / (double)copies;//Obliczanie czasu trwania operacji na tablicy
    //cout<<find_index;//Wypisuje indeks, na którym znaleziono zadaną wartość. Podobnie można zrobić dla listy wiązanej.

    // Test wydajności dla listy wiązanej ze wskaźnikiem na ostatni element (należy wybrać odpowiednią funkcję)
    auto start_lists = chrono::high_resolution_clock::now();//Czas start
    for (int i = 0; i < copies; i++) {
        //array_of_lists[i].add_back(1);//dodawanie wartości np. 1 na koniec
        //array_of_lists[i].add_front(1);//Dodawanie wartości np. 1 na początek
        //array_of_lists[i].add_at(1, 5);//Dodawanie wartości np. 1 na wybrany indeks np. 5
        //array_of_lists[i].remove_back();//Usuwanie z końca
        //array_of_lists[i].remove_front();//Usuwanie z początku
        //array_of_lists[i].remove_at(5);//Usuwanie z wybranego indeksu np. 5
        array_of_lists[i].find(0);//Wyszukiwanie zadanej wartości np. 0
    }
    auto end_lists = chrono::high_resolution_clock::now();//Czas stop
    double list_time = chrono::duration<double>(end_lists - start_lists).count() / (double)copies;//Obliczanie czasu trwania operacji na liście

    ///Do wyboru odpowiedni podpis!
    //cout << "Sredni czas dodania elementu:\n";
    //cout << "Sredni czas usuwania elementu:\n";
    cout << "Sredni czas wyszukiwania elementu:\n";
    cout << "Tablica tablic: " << array_time << "s na operacje\n";
    cout << "Tablica list: " << list_time << "s na operacje\n";

    // Zwolnienie pamięci
    for (int i = 0; i < copies; i++) {
        delete[] array_of_arrays[i];
    }
    delete[] array_of_arrays;
    delete[] array_sizes;
    delete[] array_capacities;
    delete[] array_of_lists;
    return 0;
}
