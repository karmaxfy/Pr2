CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
TARGET = pr2
SRC = src/main.cpp src/math_utils.cpp src/file_utils.cpp src/fermat.cpp src/euclid_cd.cpp src/inverse.cpp src/hughes_file.cpp src/mitm_attack.cpp src/fraction_equation.cpp src/rf_crypto_standards.cpp src/text_file.cpp

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run1: all
	./$(TARGET) 1
run2: all
	./$(TARGET) 2
run3: all
	./$(TARGET) 3
run4: all
	./$(TARGET) 4
run5: all
	./$(TARGET) 5
run6: all
	./$(TARGET) 6
run7: all
	./$(TARGET) 7

clean:
	rm -f $(TARGET)
	rm -f data/encrypted_hughes.txt data/decrypted.txt
