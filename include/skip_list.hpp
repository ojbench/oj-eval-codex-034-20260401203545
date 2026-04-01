#ifndef SKIP_LIST_HPP
#define SKIP_LIST_HPP

#include <vector>
#include <cstdint>
#include <limits>

// A generic skip list supporting insert, search, deleteItem.
// Only assumes T supports operator<. Equality is derived from <.

template <typename T>
class SkipList {
private:
    struct Node {
        T value;
        std::vector<Node*> next;
        explicit Node(const T& val, int lvl) : value(val), next(static_cast<size_t>(lvl), nullptr) {}
    };

    // Sentinel head with max level pointers
    Node* head;
    int level;             // Current highest level count (1..maxLevel)
    static constexpr int maxLevel = 32; // Enough for up to ~1e9 elements with p=0.5

    // Xorshift RNG for fast deterministic level generation
    uint64_t rng_state;

    static bool less_than(const T& a, const T& b) { return a < b; }
    static bool equal_to(const T& a, const T& b) { return !less_than(a,b) && !less_than(b,a); }

    uint32_t xorshift32() {
        uint64_t x = rng_state;
        x ^= x << 13;
        x ^= x >> 7;
        x ^= x << 17;
        rng_state = x;
        return static_cast<uint32_t>(x);
    }

    int randomLevel() {
        // Geometric distribution with p=0.5 for each level
        int lvl = 1;
        // Use bits of RNG to simulate coin flips
        uint32_t r = xorshift32();
        while ((r & 1u) && lvl < maxLevel) {
            ++lvl;
            r >>= 1;
            if (!r) r = xorshift32();
        }
        return lvl;
    }

public:
    SkipList()
        : head(new Node(T{}, maxLevel)), level(1), rng_state(0x9E3779B97F4A7C15ull) {}

    ~SkipList() {
        // Delete all nodes starting from level 0
        Node* cur = head->next[0];
        while (cur) {
            Node* nxt = cur->next[0];
            delete cur;
            cur = nxt;
        }
        delete head;
    }

    void insert(const T& item) {
        Node* update[maxLevel];
        Node* cur = head;
        for (int i = level - 1; i >= 0; --i) {
            while (cur->next[i] && less_than(cur->next[i]->value, item)) cur = cur->next[i];
            update[i] = cur;
        }

        cur = cur->next[0];
        if (cur && equal_to(cur->value, item)) {
            // already exists, do nothing
            return;
        }

        int newLevel = randomLevel();
        if (newLevel > level) {
            for (int i = level; i < newLevel; ++i) update[i] = head;
            level = newLevel;
        }

        Node* node = new Node(item, newLevel);
        for (int i = 0; i < newLevel; ++i) {
            node->next[i] = update[i]->next[i];
            update[i]->next[i] = node;
        }
    }

    bool search(const T& item) {
        Node* cur = head;
        for (int i = level - 1; i >= 0; --i) {
            while (cur->next[i] && less_than(cur->next[i]->value, item)) cur = cur->next[i];
        }
        cur = cur->next[0];
        return cur && equal_to(cur->value, item);
    }

    void deleteItem(const T& item) {
        Node* update[maxLevel];
        Node* cur = head;
        for (int i = level - 1; i >= 0; --i) {
            while (cur->next[i] && less_than(cur->next[i]->value, item)) cur = cur->next[i];
            update[i] = cur;
        }
        cur = cur->next[0];
        if (!cur || !equal_to(cur->value, item)) return; // not found

        int nodeLevel = static_cast<int>(cur->next.size());
        for (int i = 0; i < nodeLevel; ++i) {
            if (update[i]->next[i] == cur) update[i]->next[i] = cur->next[i];
        }
        delete cur;
        // Decrease current level if top levels become empty
        while (level > 1 && head->next[level - 1] == nullptr) --level;
    }
};

#endif // SKIP_LIST_HPP
