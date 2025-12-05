#include <iostream>
#include <windows.h>
using namespace std;
//класична FIFO-черга на однозв’язному списку
template<typename T>
class queue {
private:
    struct node {
        T data;
        node* next;
        node(const T& value, node* n = nullptr) : data(value), next(n) {}
    };
    node* front; // вказівник на перший елемент
    node* rear; // вказівник на останній елемент
    int count; // кількість елементів у черзі
public:
    //конструктори та деструктор
    queue(); // конструктор за замовчуванням
    queue(const queue& other); // конструктор копіювання
    queue(queue&& other); // конструктор переміщення
    queue& operator=(const queue& other); // оператор присвоєння (копіювання)
    queue& operator=(queue&& other); // оператор присвоєння (переміщення)
    ~queue(); // деструктор
    //основні методи черги
    void enqueue(const T& value); // додати елемент у кінець
    T dequeue(); // видалити і повернути перший елемент
    T& peek(); // повернути посилання на перший елемент
    const T& peek() const; // константна версія для читання
    //методи контейнера
    bool empty() const; //перевірка на порожність
    int size() const;   //кількість елементів
    void clear();       //очистити чергу
};

// РЕАЛІЗАЦІЯ
//конструктор за замовчуванням
//призначення: створює порожню чергу
//алгоритм: ініціалізує вказівники значенням nullptr та лічильник нулем
template<typename T>
queue<T>::queue() : front(nullptr), rear(nullptr), count(0) {}

//конструктор копіювання
//призначення: створює копію існуючої черги
//алгоритм: проходить по всій вихідній черзі і додає елементи через enqueue
template<typename T>
queue<T>::queue(const queue& other) : front(nullptr), rear(nullptr), count(0) {
    for (node* cur = other.front; cur != nullptr; cur = cur->next)
        enqueue(cur->data);
}

//конструктор переміщення
//призначення: ефективно переносить вміст іншої черги
//алгоритм: копіює вказівники і обнуляє джерело
template<typename T>
queue<T>::queue(queue&& other) : front(other.front), rear(other.rear), count(other.count) {
    other.front = other.rear = nullptr;
    other.count = 0;
}

//оператор присвоєння (копіювання)
//призначення: копіює вміст іншої черги в поточну
//алгоритм: очищає поточну чергу, потім додає всі елементи з іншої
template<typename T>
queue<T>& queue<T>::operator=(const queue& other) {
    if (this != &other) {
        clear();
        for (node* cur = other.front; cur != nullptr; cur = cur->next)
            enqueue(cur->data);
    }
    return *this;
}

//оператор присвоєння (переміщення)
//призначення: ефективно переносить вміст
//алгоритм: очищає поточну чергу, копіює вказівники, обнуляє джерело
template<typename T>
queue<T>& queue<T>::operator=(queue&& other) {
    if (this != &other) {
        clear();
        front = other.front;
        rear = other.rear;
        count = other.count;
        other.front = other.rear = nullptr;
        other.count = 0;
    }
    return *this;
}

//деструктор
//призначення: звільняє всю виділену пам'ять
//алгоритм: викликає clear()
template<typename T>
queue<T>::~queue() {
    clear();
}

//додає елемент у кінець черги.
//призначення: вставити новий елемент у чергу
//алгоритм: створює новий вузол, якщо черга порожня — front і rear вказують на нього,
//інакше додає в кінець і оновлює rear
//складність O(1)
template<typename T>
void queue<T>::enqueue(const T& value) {
    node* new_node = new node(value);
    if (empty()) {
        front = rear = new_node;
    }
    else {
        rear->next = new_node;
        rear = new_node;
    }
    ++count;
}

//видаляє і повертає перший елемент.
//призначення: отримати і видалити перший елемент
//алгоритм: зберігає значення, видаляє вузол, оновлює front, rear (якщо треба) і лічильник
//складність O(1)
template<typename T>
T queue<T>::dequeue() {
    node* temp = front;
    T value = temp->data;
    front = front->next;
    if (front == nullptr) rear = nullptr;
    delete temp;
    --count;
    return value;
}

//повертає посилання на перший елемент
//призначення: дозволити читання і запис першого елемента без видалення
template<typename T>
T& queue<T>::peek() {
    return front->data;
}

//константна версія для читання
template<typename T>
const T& queue<T>::peek() const {
    return front->data;
}

//перевірка на порожність
template<typename T>
bool queue<T>::empty() const {
    return count == 0;
}

//повертає кількість елементів
template<typename T>
int queue<T>::size() const {
    return count;
}

//повне очищення черги
//призначення: видалити всі елементи
//алгоритм: послідовно видаляє вузли з початку, поки черга не стане порожньою
template<typename T>
void queue<T>::clear() {
    while (front != nullptr) {
        node* temp = front;
        front = front->next;
        delete temp;
    }
    rear = nullptr;
    count = 0;
}


//ЗАДАЧА-ПРИКЛАД
//"Хвиля", необхідно "інфікувати" сусідні елементи двовимірного масиву, показав на якому кроці число було інфікованно.
//базується на принципі Гюйгенса 

//власна структура
template<typename T1, typename T2, typename T3>
struct three {
    T1 first;   // y
    T2 second;  // x
    T3 step;    // крок
};
//функція інфікування
void inf(queue<three<int, int, int>>& q, int** a, int n, int y, int x, int step) {
    if (y >= 0 && y < n && x >= 0 && x < n && a[y][x] == 0) {
        a[y][x] = step + 1;
        q.enqueue({ y, x, step + 1 });
    }
}
//тест1
int main() {
    const int n = 2500; //2500 на 2500

    int** a = new int* [n];
    for (int i = 0; i < n; ++i)
        a[i] = new int[n](); // всі нулі

    queue<three<int, int, int>> q;

    // стартові точки інфікації
    a[721][1295] = 1;
    a[437][319] = 1;
    //ще приклади
    //a[572][121] = 1;
    //a[731][493] = 1;

    q.enqueue({ 721, 1295, 1 });
    q.enqueue({ 437, 319, 1 });
    //ще приклади
    //q.enqueue({ 572, 121, 1 });
    //q.enqueue({ 731, 493, 1 });

    clock_t start = clock(); //початок відкліку
    int max_step = 1; //лічильник кроків

    while (!q.empty()) {
        three<int, int, int> p = q.dequeue();
        int y = p.first;
        int x = p.second;
        int step = p.step;

        if (step > max_step) max_step = step;

        inf(q, a, n, y - 1, x - 1, step);
        inf(q, a, n, y - 1, x, step);
        inf(q, a, n, y - 1, x + 1, step);
        inf(q, a, n, y, x - 1, step);
        inf(q, a, n, y, x + 1, step);
        inf(q, a, n, y + 1, x - 1, step);
        inf(q, a, n, y + 1, x, step);
        inf(q, a, n, y + 1, x + 1, step);
    }

    clock_t end = clock(); //кінець відліку
    //обчислення в мс, Cloks для перетворення тактовних тіків процессора в сек. 
    double time_ms = 1000.0 * (end - start) / CLOCKS_PER_SEC;

    system("cls");
    cout << "infection: 2" << endl;
    cout << "2500x2500 matrix" << endl;
    cout << "Time: " << time_ms << " ms" << endl;
    cout << "Max step: " << max_step << endl;

    // очищення пам'яті
    for (int i = 0; i < n; ++i) delete[] a[i];
    delete[] a;

    return 0;
}