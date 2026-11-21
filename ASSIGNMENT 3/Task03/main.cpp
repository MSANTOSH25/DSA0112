#include <iostream>
#include <string>

class Event {
    int rows, seats;
    char** map;

public:
    Event(int r, int s) : rows(r > 0 ? r : 0), seats(s > 0 ? s : 0), map(nullptr) {
        if (rows > 0 && seats > 0) {
            map = new char*[rows]{};
            for (int i = 0; i < rows; ++i) {
                map[i] = new char[seats];
                for (int j = 0; j < seats; ++j) map[i][j] = 'O';
            }
        }
        std::cout << "Created event (" << rows << " x " << seats << ")\n";
    }

    Event(const Event&) = delete;
    Event& operator=(const Event&) = delete;

    bool book(int r, int s) {
        if (!map || r < 0 || r >= rows || s < 0 || s >= seats || map[r][s] == 'X')
            return false;
        map[r][s] = 'X';
        return true;
    }

    bool cancel(int r, int s) {
        if (!map || r < 0 || r >= rows || s < 0 || s >= seats || map[r][s] == 'O')
            return false;
        map[r][s] = 'O';
        return true;
    }

    void print() const {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < seats; ++j) std::cout << map[i][j] << ' ';
            std::cout << '\n';
        }
    }

    ~Event() {
        if (map) {
            for (int i = 0; i < rows; ++i) delete[] map[i];
            delete[] map;
        }
        std::cout << "Closed event; released " << rows * seats << " seat cells\n";
    }
};

int main() {
    Event local(2, 3);
    local.book(0, 1);
    local.print();

    {
        Event blockEvent(1, 2);
        blockEvent.book(0, 0);
    }

    Event* heapEvent = new Event(1, 2);
    heapEvent->book(0, 1);
    delete heapEvent;

    Event emptyEvent(0, 5);
}
