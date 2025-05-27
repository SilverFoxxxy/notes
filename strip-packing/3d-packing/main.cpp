#include "packing.cpp"

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

class LowestFitPacking : public PackingAlgorithm {
private:
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

        position_list.push_back({
            min(pos1.x + corner_item1.dx, pos2.x + corner_item2.dx),
            min(pos2.y + corner_item1.dy, pos2.y + corner_item2.dy),
            0
        });

        // if (!check_boundary(item, position_list.back(), box)) {
        //     position_list.pop_back();
        // }
    }

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

            vector <Point> position_list;
            position_list.push_back({0, 0, 0});
            Item item0 = {0, 0, 0, 0};
            for (int j = 0; j < i; j++) {
                // add_position(box, item, item0, items[j], position_list[0], positions[j], position_list);
                position_list.push_back({positions[j].x + items[j].dx, positions[j].y});
                position_list.push_back({positions[j].x, positions[j].y + items[j].dy});

                if (positions[j].x < item.dx) {
                    position_list.push_back({0, positions[j].y + items[j].dy, 0});
                }
                if (positions[j].y < item.dy) {
                    // cout << "HERE!\n";
                    position_list.push_back({positions[j].x + items[j].dx, 0, 0});
                }
                for (int k = 0; k < i; k++) {
                    add_position(box, item, items[j], items[k], positions[j], positions[k], position_list);
                }
            }

            int best_x = 0;
            int best_y = 0;
            int best_z = 1e9;

            // for (int j = 0; j < i; j++) {
            //     cout << positions[j].z << " ";
            // } cout << " = positions\n";

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
                new LowestFitPacking()
            };

            Solution solution = algorithms[solution_type]->get_solution(task);

            // TODO: parse moves
            *move_count = 0;

            for (int i = 0; i < n; i++) {
                pos[3 * i] = solution.final_state.positions[i].x;
                pos[3 * i + 1] = solution.final_state.positions[i].y;
                pos[3 * i + 2] = solution.final_state.positions[i].z;
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
