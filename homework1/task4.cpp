#include <iostream>
#include <vector>
#include <utility>
#include <stdexcept>

class Heap {
private:
    std::vector<int> data;
    bool is_max;
    void sift_up(int i);
    void sift_down(int i);
    bool higher_priority(int a, int b);
public:
    explicit Heap(bool is_max) : is_max(is_max) {}
    void push(int value);
    int pop();
    int top() const;
    int size() const;
    bool empty() const;
};

bool Heap::higher_priority(int a, int b) {
    return is_max ? a > b : a < b;
}

int Heap::top() const {
    if (data.empty()) {
        throw std::out_of_range("Heap is empty");
    }
    return data[0];
}

int Heap::size() const {
    return static_cast<int>(data.size());
}

bool Heap::empty() const {
    return data.empty();
}

void Heap::sift_up(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (!higher_priority(data[i], data[parent])) {
            break;
        }
        std::swap(data[parent], data[i]);
        i = parent;
    }
}

void Heap::push(int value) {
    data.push_back(value);
    sift_up(static_cast<int>(data.size()) - 1);
}

void Heap::sift_down(int i) {
    int n = data.size();
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;
        if ((left < n) && (higher_priority(data[left], data[largest]))) {
            largest = left;
        }
        if ((right < n) && (higher_priority(data[right], data[largest]))) {
            largest = right;
        }
        if (largest == i) {
            break;
        }
        std::swap(data[i], data[largest]);
        i = largest;
    }
}

int Heap::pop() {
    if (data.empty()) {
        throw std::out_of_range("Heap is empty");
    }
    int result = data[0];
    data[0] = data.back();
    data.pop_back();
    if (!data.empty()) {
        sift_down(0);
    }
    return result;
}


int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Heap left(true);
    Heap right(false);

    int n;
    std::cin >> n;
    int current;
    for (int i = 0; i < n; i++) {
        std::cin >> current;
        if ((left.empty()) || (current <= left.top())) {
            left.push(current);
            if (left.size() - right.size() > 1) {
                right.push(left.pop());
            } 
        } else {
            right.push(current);
            if (right.size() - left.size() > 0) {
                left.push(right.pop());
            }
        }
        std::cout << left.top() << ' ';
    }

    return 0;
}