import ctypes
import numpy as np

# g++ -shared -fPIC -o libmodule.so module.cpp
lib = ctypes.CDLL('./libmodule.so')
lib.process_array.argtypes = [
    ndpointer(ctypes.c_double, flags="C_CONTIGUOUS"),
    ndpointer(ctypes.c_double, flags="C_CONTIGUOUS"),
    ctypes.c_int
]

def process_array(input_array):
    output_array = np.empty_like(input_array)
    lib.process_array(input_array, output_array, len(input_array))
    return output_array

# Пример использования
if __name__ == "__main__":
    arr = np.array([1.0, 2.0, 3.0], dtype=np.float64)
    print("Input:", arr)
    print("Output:", process_array(arr))
