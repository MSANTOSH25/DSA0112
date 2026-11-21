#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

struct Time {
    int seconds;
    Time& operator++() { ++seconds; return *this; }             // prefix
    Time operator++(int) { Time old = *this; ++seconds; return old; } // postfix
    bool operator==(const Time& other) const { return seconds == other.seconds; }
    bool operator<(const Time& other) const { return seconds < other.seconds; }
};

class Playlist {
    std::vector<std::string> tracks;
public:
    Playlist(std::initializer_list<std::string> names) : tracks(names) {}
    std::string& operator[](std::size_t i) {
        if (i >= tracks.size()) throw std::out_of_range("Track index out of range");
        return tracks[i];
    }
    const std::string& operator[](std::size_t i) const {
        if (i >= tracks.size()) throw std::out_of_range("Track index out of range");
        return tracks[i];
    }
    std::size_t size() const { return tracks.size(); }
};

int main() {
    Time t{5};
    Time a = t++;
    Time b = ++t;
    std::cout << a.seconds << ' ' << b.seconds << ' ' << t.seconds << '\n';

    Playlist list{"Song A", "Song B", "Song C"};
    for (std::size_t i = 0; i < list.size(); ++i)
        std::cout << list[i] << '\n';

    try { std::cout << list[3] << '\n'; }
    catch (const std::out_of_range& e) { std::cout << e.what() << '\n'; }
}
