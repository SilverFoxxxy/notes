#include "packing.cpp"

typedef long long ll;

class SimpleTowerPacking : public PackingAlgorithm {
private:
    virtual Solution solve(const PackingState& task) {
        PackingState packing_state = task;
        State state;
        task.get_state<State>(state);

        State state_to_change = state;

        // const PackingState task;
        // const int max_moves_count = 0;
        // vector <PackingAction> moves;
        // PackingState final_state;
        // int height = 0;
        vector <PackingAction> moves;
        int cur_height = 0;
        for (int i = 0; i < state.items.size(); i++) {
            PackingAction move = { ActionType::PLACE, i, {0, 0, cur_height} };
            moves.push_back(move);
            packing_state.apply_move(move);
            cur_height += state.items[i].dz;
        }

        int max_moves_count = 0;

        Solution solution(
            task,
            max_moves_count,
            moves,
            packing_state,
            cur_height
        );

        return solution;
    }

public:
    struct State {
        Item box;
        int n;
        std::vector<Item> items;
        std::vector<Point> positions;
        std::vector<bool> is_placed;
    };
};

class SimpleRandomPacking : public PackingAlgorithm {
private:
    virtual Solution solve(const PackingState& task) {
        PackingState packing_state = task;
        State state;
        task.get_state<State>(state);

        State state_to_change = state;

        // const PackingState task;
        // const int max_moves_count = 0;
        // vector <PackingAction> moves;
        // PackingState final_state;
        // int height = 0;
        vector <PackingAction> moves;
        int cur_height = 0;
        const Item& box = state.box;
        auto& is_placed = state.is_placed;
        const auto& items = state.items;
        auto& positions = state.positions;

        for (int i = 0; i < state.items.size(); i++) {
            const Item& item = items[i];
            int x = rand() % (box.dx - item.dx + 1);
            int y = rand() % (box.dy - item.dy + 1);

            int z = 0;
            for (int j = 0; j < i; j++) {
                if (has_collision_by_coord(x, item.dx, positions[j].x, items[j].dx) &&
                    has_collision_by_coord(y, item.dy, positions[j].y, items[j].dy)) {
                    z = max(z, positions[j].z + items[j].dz);
                }
            }
            cur_height = max(z + item.dz, cur_height);

            PackingAction move = { ActionType::PLACE, i, {x, y, z} };
            moves.push_back(move);
            packing_state.apply_move(move);
            state.is_placed[i] = true;
            state.positions[i] = {x, y, z};
        }

        int max_moves_count = 0;

        Solution solution(
            task,
            max_moves_count,
            moves,
            packing_state,
            cur_height
        );

        return solution;
    }

public:
    struct State {
        Item box;
        int n;
        std::vector<Item> items;
        std::vector<Point> positions;
        std::vector<bool> is_placed;
    };
};

class GreedyRandomPacking : public PackingAlgorithm {
private:
    virtual Solution solve(const PackingState& task) {
        PackingState packing_state = task;
        State state;
        task.get_state<State>(state);

        State state_to_change = state;

        // const PackingState task;
        // const int max_moves_count = 0;
        // vector <PackingAction> moves;
        // PackingState final_state;
        // int height = 0;
        vector <PackingAction> moves;
        int cur_height = 0;
        const Item& box = state.box;
        auto& is_placed = state.is_placed;
        const auto& items = state.items;
        auto& positions = state.positions;

        for (int i = 0; i < state.items.size(); i++) {
            const Item& item = items[i];

            int best_x = 0;
            int best_y = 0;
            int best_z = 1e9;
            for (int _ = 0; _ < 1000; _++) {
                int x = rand() % (box.dx - item.dx + 1);
                int y = rand() % (box.dy - item.dy + 1);

                int z = 0;
                for (int j = 0; j < i; j++) {
                    if (has_collision_by_coord(x, item.dx, positions[j].x, items[j].dx) &&
                        has_collision_by_coord(y, item.dy, positions[j].y, items[j].dy)) {
                        z = max(z, positions[j].z + items[j].dz);
                        if (z > best_z) break;
                    }
                }

                if (z < best_z ||
                    z == best_z && x < best_x ||
                    z == best_z && x == best_x && y < best_y) {
                    best_z = z;
                    best_x = x;
                    best_y = y;
                }
            }

            cur_height = max(best_z + item.dz, cur_height);
            PackingAction move = { ActionType::PLACE, i, {best_x, best_y, best_z} };
            moves.push_back(move);
            packing_state.apply_move(move);
            state.is_placed[i] = true;
            state.positions[i] = {best_x, best_y, best_z};
        }

        int max_moves_count = 0;

        Solution solution(
            task,
            max_moves_count,
            moves,
            packing_state,
            cur_height
        );

        return solution;
    }

public:
    struct State {
        Item box;
        int n;
        std::vector<Item> items;
        std::vector<Point> positions;
        std::vector<bool> is_placed;
    };
};

void add_position(
    const Item& box,
    const Item& item,
    const Item& corner_item1,
    const Item& corner_item2,
    const Point& pos1,
    const Point& pos2,
    vector <Point>& position_list
) {
    Item check_item = corner_item1;
    check_item.dx += 2 * item.dx;
    check_item.dy += 2 * item.dy;
    check_item.dz += 1e9;

    Point check_pos = pos1;
    check_pos.x -= item.dx;
    check_pos.y -= item.dy;
    check_pos.z -= 5e8;

    if (!has_collision(check_item, check_pos, corner_item2, pos2)) {
        return;
    }

    vector <int> x_vec = {
        min(pos1.x, pos2.x),
        min(pos1.x + corner_item1.dx, pos2.x + corner_item2.dx),
        max(pos1.x, pos2.x) - item.dx,
        max(pos1.x + corner_item1.dx, pos2.x + corner_item2.dx) - item.dx
    };

    vector <int> y_vec = {
        min(pos1.y, pos2.y),
        min(pos1.y + corner_item1.dy, pos2.y + corner_item2.dy),
        max(pos1.y, pos2.y) - item.dy,
        max(pos1.y + corner_item1.dy, pos2.y + corner_item2.dy) - item.dy
    };

    for (int x: x_vec) {
        for (int y: y_vec) {
            position_list.push_back({
                x,
                y,
                0
            });
        }
    }
}

void get_corner_positions(
    const Item& box,
    const Item& item,
    const vector <bool>& is_placed,
    const vector <Point>& positions_arg,
    const vector <Item>& items_arg,
    vector <Point>& position_list
) {
    position_list.push_back({0, 0, 0});
    Item item0 = {0, 0, 0, 0};

    vector <Item> items = items_arg;
    vector <Point> positions = positions_arg;

    items.insert(
        items.end(),
        {
            {-1, 1, box.dy, (int)5e8},
            {-1, 1, box.dy, (int)5e8},
            {-1, box.dx, 1, (int)5e8},
            {-1, box.dx, 1, (int)5e8}
        }
    );

    positions.insert(
        positions.end(),
        {
            {-1, 0, 0},
            {box.dx, 0, 0},
            {0, -1, 0},
            {0, box.dy}
        }
    );

    for (int j = 0; j < items.size(); j++) {
        if (j < items.size() - 4 && !is_placed[j]) {
            continue;
        }
        // add_position(box, item, item0, items[j], position_list[0], positions[j], position_list);

        vector <int> x_vec = {
            positions[j].x - item.dx,
            positions[j].x,
            positions[j].x + items[j].dx - item.dx,
            positions[j].x + items[j].dx
        };

        vector <int> y_vec = {
            positions[j].y - item.dy,
            positions[j].y,
            positions[j].y + items[j].dy - item.dy,
            positions[j].y + items[j].dy
        };

        for (int xi = 0; xi < x_vec.size(); xi++) {
            for (int yi = 0; yi < y_vec.size(); yi++) {
                if ((xi == 0 || xi + 1 == x_vec.size()) &&
                    (yi == 0 || yi + 1 == y_vec.size())) {
                    continue;
                }
                position_list.push_back({ x_vec[xi], y_vec[yi], 0 });
            }
        }

        for (int k = 0; k < items.size(); k++) {
            if (k < items.size() - 4 && !is_placed[j]) {
                continue;
            }
            add_position(box, item, items[j], items[k], positions[j], positions[k], position_list);
        }
    }
}

class LowestFitPacking : public PackingAlgorithm {
private:
    virtual Solution solve(const PackingState& task) {
        PackingState packing_state = task;
        State state;
        task.get_state<State>(state);

        State state_to_change = state;

        // const PackingState task;
        // const int max_moves_count = 0;
        // vector <PackingAction> moves;
        // PackingState final_state;
        // int height = 0;
        vector <PackingAction> moves;
        int cur_height = 0;
        const Item& box = state.box;
        auto& is_placed = state.is_placed;
        const auto& items = state.items;
        auto& positions = state.positions;

        for (int i = 0; i < state.items.size(); i++) {
            const Item& item = items[i];

            int best_x = 0;
            int best_y = 0;
            int best_z = 1e9;

            // for (int j = 0; j < i; j++) {
            //     cout << positions[j].z << " ";
            // } cout << " = positions\n";

            vector <Point> position_list;
            get_corner_positions(
                box,
                item,
                is_placed,
                positions,
                items,
                position_list
            );

            for (auto [x, y, z]: position_list) {
                // cout << x << ' ' << y << ' ' << z << ' ' << best_z << " = position\n";
                z = 0;
                if (!check_boundary(item, {x, y, z}, box)) {
                    // cout << "oh no\n";
                    continue;
                }
                for (int j = i - 1; j >= 0; j--) {
                    if (has_collision_by_coord(x, item.dx, positions[j].x, items[j].dx) &&
                        has_collision_by_coord(y, item.dy, positions[j].y, items[j].dy)) {
                        // cout << j << ' ' << positions[j].z + items[j].dz << "\n";
                        z = max(z, positions[j].z + items[j].dz);
                        if (z > best_z) break;
                    }
                }
                // cout << z << ' ' << best_z << "\n";

                if (z < best_z ||
                    z == best_z && x < best_x ||
                    z == best_z && x == best_x && y < best_y) {
                    best_z = z;
                    best_x = x;
                    best_y = y;
                }
            }

            cur_height = max(best_z + item.dz, cur_height);

            PackingAction move = { ActionType::PLACE, i, {best_x, best_y, best_z} };
            moves.push_back(move);
            packing_state.apply_move(move);
            state.is_placed[i] = true;
            state.positions[i] = {best_x, best_y, best_z};
        }

        int max_moves_count = 0;

        Solution solution(
            task,
            max_moves_count,
            moves,
            packing_state,
            cur_height
        );

        return solution;
    }

public:
    struct State {
        Item box;
        int n;
        std::vector<Item> items;
        std::vector<Point> positions;
        std::vector<bool> is_placed;
    };
};

class BestVolumeCoveragePacking : public PackingAlgorithm {
private:
    virtual Solution solve(const PackingState& task) {
        PackingState packing_state = task;
        State state;
        task.get_state<State>(state);

        State state_to_change = state;

        vector <PackingAction> moves;
        int cur_height = 0;
        const Item& box = state.box;
        auto& is_placed = state.is_placed;
        const auto& items = state.items;
        auto& positions = state.positions;

        for (int i = 0; i < state.items.size(); i++) {
            cout << "item id = " << i << endl;
            const Item& item = items[i];

            int best_x = 0;
            int best_y = 0;
            int best_z = 1e9;
            ll best_coverage = 1e18;

            vector <Point> position_list;
            get_corner_positions(
                box,
                item,
                is_placed,
                positions,
                items,
                position_list
            );

            for (auto [x, y, z]: position_list) {
                z = 0;
                if (!check_boundary(item, {x, y, z}, box)) {
                    continue;
                }
                for (int j = i - 1; j >= 0; j--) {
                    if (has_collision_by_coord(x, item.dx, positions[j].x, items[j].dx) &&
                        has_collision_by_coord(y, item.dy, positions[j].y, items[j].dy)) {
                        z = max(z, positions[j].z + items[j].dz);
                    }
                }

                ll coverage = item.dx * item.dy * z;

                Item covered_space = {-1, item.dx, item.dy, z};

                for (int j = i - 1; j >= 0; j--) {
                    coverage -= get_collision_volume(covered_space, {x, y, 0}, items[j], positions[j]);
                }

                if (coverage < best_coverage) {
                    best_coverage = coverage;
                    best_x = x;
                    best_y = y;
                    best_z = z;
                }
            }

            cur_height = max(best_z + item.dz, cur_height);

            PackingAction move = { ActionType::PLACE, i, {best_x, best_y, best_z} };
            moves.push_back(move);
            packing_state.apply_move(move);
            state.is_placed[i] = true;
            state.positions[i] = {best_x, best_y, best_z};
        }

        int max_moves_count = 0;

        Solution solution(
            task,
            max_moves_count,
            moves,
            packing_state,
            cur_height
        );

        return solution;
    }

public:
    struct State {
        Item box;
        int n;
        std::vector<Item> items;
        std::vector<Point> positions;
        std::vector<bool> is_placed;
    };
};

extern "C" {
    struct ErrorInfo {
        char* message;
        int error_code;
    };

    void process_array(double* in_array, double* out_array, int size) {
        for (int i = 0; i < size; ++i) {
            out_array[i] = in_array[i] * 2.0;
        }
    }

    void process_task(
        int n,
        int w, int l,
        int max_move_count,
        int *items, /* [dx0, dy0, dz0, dx1, dy1, dz1, ... ] */
        int solution_type,
        int *move_count,
        int **moves, /* [item_id, x, y, z, item_id, x, y, z, ...] */
        int *pos, /* [x, y, z, x, y, z, x, y, z, x, y, z] */
        ErrorInfo *error
    ) {
        try {
            vector <Item> items_v(n);
            for (int i = 0; i < n; i++) {
                items_v[i] = { i, items[i * 3], items[i * 3 + 1], items[i * 3 + 2] };
            }

            Item box = { -1, w, l, 0 };

            PackingState task(box, items_v);

            vector <PackingAlgorithm*> algorithms = {
                new SimpleTowerPacking(),
                new SimpleRandomPacking(),
                new GreedyRandomPacking(),
                new LowestFitPacking(),
                new BestVolumeCoveragePacking()
            };

            if (solution_type >= algorithms.size()) {
                return;
            }

            Solution solution = algorithms[solution_type]->get_solution(task);

            // TODO: parse moves
            *move_count = 0;

            for (int i = 0; i < n; i++) {
                pos[3 * i] = solution.final_state.positions[i].x;
                pos[3 * i + 1] = solution.final_state.positions[i].y;
                pos[3 * i + 2] = solution.final_state.positions[i].z;
            }

            for (auto x: algorithms) {
                delete x;
            }
        } catch (const std::exception& e) {
            // Copy error message to return structure
            error->message = strdup(e.what());
            error->error_code = -1;
        } catch (...) {
            error->message = strdup("Unknown exception occurred");
            error->error_code = -2;
        }
    }

    void free_moves(int *moves) {
        delete[] moves;
    }

    void free_error(ErrorInfo* error) {
        if (error && error->message) {
            free(error->message);
        }
    }
}

// int main() {
//     // int n,
//     // int w, int l,
//     // int max_move_count
//     // int *items, /* [dx0, dy0, dz0, dx1, dy1, dz1, ... ] */
//     // int *move_count,
//     // int **moves, /* [item_id, x, y, z, item_id, x, y, z, ...] */
//     // int *pos /* [x, y, z, x, y, z, x, y, z, x, y, z] */
//     int n = 2;
//     int w = 10, l = 10;
//     int max_move_count = 0;
//     int *items = new int[3 * n];
//     items[0] = 3;
//     items[1] = 4;
//     items[2] = 2;
//     items[3] = 2;
//     items[4] = 5;
//     items[5] = 4;

//     int *pos = new int[3 * n];

//     int move_count;
//     int *moves;

//     process_task(
//         n, w, l,
//         max_move_count,
//         items,
//         &move_count,
//         &moves,
//         pos
//     );

//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j < 3; j++) {
//             cout << pos[3 * i + j] << ' ';
//         }
//         cout << "\n";
//     }
// }
