from dataclasses import dataclass
from typing import List, Tuple, Dict, Optional
import numpy as np

# ====================
# 1. Хранение данных
# ====================

"""
Формат входных данных (пример):
10 10  # Ширина и длина контейнера
3      # Количество предметов
2 3 1  # Ширина, длина, высота предмета 1
4 2 2  # Предмет 2
1 5 3  # Предмет 3
2      # Параметр K (макс. перестановок на шаг)
"""

@dataclass
class TestCase:
    container_width: int
    container_length: int
    items: List[Tuple[int, int, int]]  # (width, length, height)
    max_repacks: int

    @classmethod
    def from_string(cls, data: str):
        lines = [line.strip() for line in data.split('\n') if line.strip()]
        w, l = map(int, lines[0].split())
        n = int(lines[1])
        items = [tuple(map(int, line.split())) for line in lines[2:2+n]]
        k = int(lines[2+n]) if len(lines) > 2+n else 0
        return cls(w, l, items, k)

# ====================
# 2. Хранение ответа
# ====================

class PackingSolution:
    def __init__(self):
        self.actions = []  # Список действий: ('place', item_idx, x, y, z) или ('move', item_idx, new_x, new_y)
        self.final_positions = []  # Финальные позиции: (x, y, z, w, l, h)

    def add_placement(self, item_idx: int, x: int, y: int, z: int):
        self.actions.append(('place', item_idx, x, y, z))

    def add_move(self, item_idx: int, new_x: int, new_y: int):
        self.actions.append(('move', item_idx, new_x, new_y))

# ====================
# 3. Базовый алгоритм
# ====================

class PackingAlgorithm:
    def __init__(self, container_width: int, container_length: int):
        self.w = container_width
        self.l = container_length
        self.items = []  # Текущие предметы: (x, y, z, w, l, h)

    def pack(self, test_case: TestCase) -> PackingSolution:
        """Основной метод, который нужно переопределять"""
        raise NotImplementedError

    def is_valid_position(self, x: int, y: int, z: int, w: int, l: int, h: int) -> bool:
        """Проверяет, что позиция допустима (с учётом гравитации и непересечения)"""
        # 1. Проверка выхода за границы контейнера
        if x < 0 or y < 0 or z < 0 or x + w > self.w or y + l > self.l:
            return False

        # 2. Проверка опоры (должна быть опора по всей площади)
        if z > 0:
            support = False
            for (ox, oy, oz, ow, ol, oh) in self.items:
                if oz + oh == z:
                    x_overlap = not (x + w <= ox or ox + ow <= x)
                    y_overlap = not (y + l <= oy or oy + ol <= y)
                    if x_overlap and y_overlap:
                        support = True
                        break
            if not support:
                return False

        # 3. Проверка пересечений с другими предметами
        for (ox, oy, oz, ow, ol, oh) in self.items:
            if (x < ox + ow and x + w > ox and
                y < oy + ol and y + l > oy and
                z < oz + oh and z + h > oz):
                return False

        return True

    def is_free(self, item_idx: int) -> bool:
        """Проверяет, свободен ли предмет (нет предметов сверху)"""
        x, y, z, w, l, h = self.items[item_idx]
        for (ox, oy, oz, ow, ol, oh) in self.items:
            if (ox < x + w and ox + ow > x and
                oy < y + l and oy + ol > y and
                oz >= z + h):
                return False
        return True

# ====================
# 4. Пример алгоритма
# ====================

class BottomLeftAlgorithm(PackingAlgorithm):
    def pack(self, test_case: TestCase) -> PackingSolution:
        solution = PackingSolution()
        for idx, (w, l, h) in enumerate(test_case.items):
            # Ищем первую допустимую позицию (жадный подход)
            placed = False
            for z in sorted({0} | {item[2] + item[5] for item in self.items}):
                for y in range(0, self.l - l + 1):
                    for x in range(0, self.w - w + 1):
                        if self.is_valid_position(x, y, z, w, l, h):
                            self.items.append((x, y, z, w, l, h))
                            solution.add_placement(idx, x, y, z)
                            placed = True
                            break
                    if placed: break
                if placed: break
        solution.final_positions = self.items.copy()
        return solution

# ====================
# 5. Валидатор решений
# ====================

def validate_solution(test_case: TestCase, solution: PackingSolution) -> bool:
    simulator = PackingAlgorithm(test_case.container_width, test_case.container_length)
    repacks_used = 0

    for action in solution.actions:
        if action[0] == 'place':
            _, item_idx, x, y, z = action
            w, l, h = test_case.items[item_idx]

            # Проверка допустимости размещения
            if not simulator.is_valid_position(x, y, z, w, l, h):
                return False

            # Проверка что предмет свободный после размещения
            simulator.items.append((x, y, z, w, l, h))
            if not simulator.is_free(len(simulator.items)-1):
                return False

        elif action[0] == 'move':
            _, item_idx, new_x, new_y = action
            if item_idx >= len(simulator.items):
                return False

            x, y, z, w, l, h = simulator.items[item_idx]

            # Проверка что предмет свободный перед перемещением
            if not simulator.is_free(item_idx):
                return False

            # Проверка новой позиции
            if not simulator.is_valid_position(new_x, new_y, z, w, l, h):
                return False

            # Обновляем позицию
            simulator.items[item_idx] = (new_x, new_y, z, w, l, h)
            repacks_used += 1

    # Проверка количества перестановок
    if repacks_used > test_case.max_repacks * len(test_case.items):
        return False

    # Проверка соответствия финальной расстановки
    if len(solution.final_positions) != len(test_case.items):
        return False

    return True

# ====================
# 6. Пример использования
# ====================

input_data = """
10 10
3
2 3 1
4 2 2
1 5 3
2
"""

test_case = TestCase.from_string(input_data)
algorithm = BottomLeftAlgorithm(test_case.container_width, test_case.container_length)
solution = algorithm.pack(test_case)

print("Решение валидно:", validate_solution(test_case, solution))
print("Финальные позиции:", solution.final_positions)