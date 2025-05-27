#include <bits/stdc++.h>

using namespace std;

struct Item {
    int id;
    int dx, dy, dz;
};

bool operator == (const Item& item, const Item& other) {
    return item.id == other.id &&
           item.dx == other.dx &&
           item.dy == other.dy &&
           item.dz == other.dz;
}

struct Point {
    int x = 0;
    int y = 0;
    int z = 0;
};

bool operator == (const Point& a, const Point& b) {
    return a.x == b.x &&
           a.y == b.y &&
           a.z == b.z;
}

enum class ActionType { PLACE, MOVE };

struct PackingAction {
    ActionType type;
    int item_id;
    Point position;
};

bool has_collision_by_coord(int ax, int adx, int bx, int bdx) {
    int l = max(ax, bx);
    int r = min(ax + adx, bx + bdx);

    return l < r;
}

bool has_collision(const Item& a, const Point& pos_a, const Item& b, const Point& pos_b) {
    if (has_collision_by_coord(pos_a.x, a.dx, pos_b.x, b.dx) &&
        has_collision_by_coord(pos_a.y, a.dy, pos_b.y, b.dy) &&
        has_collision_by_coord(pos_a.z, a.dz, pos_b.z, b.dz)) {
        return true;
    }
    return false;
}

bool check_boundary(const Item& item, const Point& pos, const Item& box) {
    if ((0 <= pos.x && pos.x + item.dx <= box.dx) &&
        (0 <= pos.y && pos.y + item.dy <= box.dy) &&
        (0 <= pos.z)) {
        return true;
    }
    return false;
}

class PackingState {
public:
    const Item box;
    const int n;
    const std::vector<Item>& items;
    std::vector<Point> positions;
    std::vector<bool> is_placed;

    bool has_collision_with_placed(const int& item_id, const Item& item, const Point& pos) const {
        for (size_t i = 0; i < items.size(); ++i) {
            if (!is_placed[i]) {
                continue;
            }

            if (has_collision(item, pos, items[i], positions[i])) {
                return true;
            }
        }
        return false;
    }

    bool has_gravity_support(const Item& item, const Point& pos) const {
        if (pos.z == 0) return true;

        for (size_t i = 0; i < items.size(); ++i) {
            if (!is_placed[i]) {
                continue;
            }

            const Point& pos_a = pos;
            const Item& a = item;

            const Point& pos_b = positions[i];
            const Item& b = items[i];

            // cout << pos_a.x << ' ' << a.dx << ' ' << pos_b.x << ' ' << b.dx << ' ' << has_collision_by_coord(pos_a.x, a.dx, pos_b.x, b.dx) << "\n";
            // cout << pos_a.y << ' ' << a.dy << ' ' << pos_b.y << ' ' << b.dy << ' ' << has_collision_by_coord(pos_a.y, a.dy, pos_b.y, b.dy) <<  "\n";
            // cout << pos_a.z << ' ' << a.dz << ' ' << pos_b.z << ' ' << b.dz << "\n";

            if (has_collision_by_coord(pos_a.x, a.dx, pos_b.x, b.dx) &&
                has_collision_by_coord(pos_a.y, a.dy, pos_b.y, b.dy) &&
                pos_a.z == pos_b.z + b.dz) {
                // cout << "YES\n";
                return true;
            }
        }

        return false;
    }

    bool is_position_free(const Item& item, const Point& pos) const {
        for (size_t i = 0; i < items.size(); ++i) {
            if (!is_placed[i]) {
                continue;
            }

            const Point& pos_a = pos;
            const Item& a = item;

            const Point& pos_b = positions[i];
            const Item& b = items[i];

            if (has_collision_by_coord(pos_a.x, a.dx, pos_b.x, b.dx) &&
                has_collision_by_coord(pos_a.y, a.dy, pos_b.y, b.dy) &&
                pos_a.z <= pos_b.z) {
                return false;
            }
        }

        return true;
    }

    bool is_item_free(const size_t item_id) const {
        const Item& item = items[item_id];
        const Point& pos = positions[item_id];
        for (size_t i = 0; i < items.size(); ++i) {
            if (!is_placed[i] || i == item_id) {
                continue;
            }

            const Point& pos_a = pos;
            const Item& a = item;

            const Point& pos_b = positions[i];
            const Item& b = items[i];

            if (has_collision_by_coord(pos_a.x, a.dx, pos_b.x, b.dx) &&
                has_collision_by_coord(pos_a.y, a.dy, pos_b.y, b.dy) &&
                pos_a.z <= pos_b.z) {
                return false;
            }
        }

        return true;
    }

    bool is_valid_position(const size_t item_id, const Point& pos) const {
        const Item& item = items[item_id];
        // Boundary check
        if (!check_boundary(item, pos, box)) {
            throw std::runtime_error("Not in boundary");
            return false;
        }

        // Collision check
        if (has_collision_with_placed(item_id, item, pos)) {
            throw std::runtime_error("Has collision with placed");
            return false;
        }

        // cerr << item.dz << ' ' << pos.z << std::endl;

        // Gravity support check
        if (!has_gravity_support(item, pos)) {
            cerr << item.dz << '#' << pos.z << std::endl;
            throw std::runtime_error("Position does not have gravity support");
            return false;
        }

        // Position is free check
        if (!is_position_free(item, pos)) {
            throw std::runtime_error("Position is not free");
            return false;
        }

        return true;
    }

public:
    PackingState(Item box, const std::vector<Item>& items) : box(box), items(items), n(items.size()) {
        positions.resize(n);
        is_placed.resize(n, false);
    }

    void place_item(size_t item_id, const Point& pos) {
        if (is_placed[item_id]) {
            throw std::runtime_error("Item is already placed");
        }
        if (!is_valid_position(item_id, pos)) {
            throw std::runtime_error("Invalid position");
        }

        positions[item_id] = pos;
        is_placed[item_id] = true;
    }

    void move_item(size_t item_id, const Point& new_pos) {
        if (item_id >= items.size()) {
            throw std::out_of_range("Invalid item index to be moved");
        }
        if (!is_item_free(item_id)) {
            throw std::runtime_error("Item is not free to be moved");
        }
        if (!is_placed[item_id]) {
            throw std::runtime_error("Item is not placed to be moved");
        }

        is_placed[item_id] = false;

        place_item(item_id, new_pos);
    }

    void apply_move(PackingAction& move) {
        if (move.type == ActionType::PLACE) {
            // cout << "PLACE: " << move.position.z << "\n";
            place_item(move.item_id, move.position);
        }
        if (move.type == ActionType::MOVE) {
            // cout << "PLACE: " << move.position.z << "\n";
            move_item(move.item_id, move.position);
        }
    }

    bool all_placed() {
        for (auto x: is_placed) {
            if (!x) return false;
        }
        return true;
    }

    bool operator==(const PackingState& other) const {
        // Compare the box, n, items, positions, and is_placed vectors
        return box == other.box &&
                n == other.n &&
                &items == &other.items && // Compare addresses since it's a reference
                positions == other.positions &&
                is_placed == other.is_placed;
    }

    template <typename S>
    void get_state(S& state) const {
        state.box = box;
        state.n = n;
        state.items = items;
        state.positions = positions;
        state.is_placed = is_placed;
    }
};

class Solution {
public:
    const PackingState task;
    const int max_moves_count = 0;
    vector <PackingAction> moves;
    PackingState final_state;
    int height = 0;

public:
    Solution(
        const PackingState& task,
        int max_moves_count,
        const vector <PackingAction>& moves,
        const PackingState& final_state,
        int height
    ) : task(task), max_moves_count(max_moves_count), moves(moves), final_state(final_state), height(height) {}

    // works in O(N * M), where N - number of items, M - number of moves
    // TODO: check that items are placed in order of coming!
    // TODO: check that no more than max_moves_count of moves is made between two new items
    bool check() {
        PackingState state = task;

        if (!final_state.all_placed()) {
            return false;
        }

        for (auto move: moves) {
            state.apply_move(move);
        }

        if (!(state == final_state)) {
            return false;
        }

        return true;
    }
};

class PackingAlgorithm {
private:
    virtual Solution solve(const PackingState& task) = 0;

public:
    Solution get_solution(const PackingState& task) {
        Solution solution = solve(task);
        if (!solution.check()) {
            throw std::runtime_error("Solution is not correct");
        }

        return solution;
    }
};
