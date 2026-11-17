# Compilador
CXX = g++
CXXFLAGS = -Wall -g -O2 -Iinclude

# Carpetas
SRC_DIR = src
APP_DIR = apps
OBJ_DIR = obj
BIN_DIR = bin

# Archivos
OBJS   = $(OBJ_DIR)/range_tree.o $(OBJ_DIR)/layered_range_tree.o $(OBJ_DIR)/interval_tree.o $(OBJ_DIR)/app_main.o
TARGET = $(BIN_DIR)/app_trees

.PHONY: all clean run

# Regla principal
all: $(TARGET)

# Enlazado (garantiza que exista bin/)
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilación (garantiza que exista obj/)
$(OBJ_DIR)/range_tree.o: $(SRC_DIR)/range_tree.cpp include/range_tree.h include/comun.h | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/layered_range_tree.o: $(SRC_DIR)/layered_range_tree.cpp include/layered_range_tree.h include/comun.h | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/interval_tree.o: $(SRC_DIR)/interval_tree.cpp include/interval_tree.h include/comun.h | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/app_main.o: $(APP_DIR)/app_main.cpp include/range_tree.h include/layered_range_tree.h include/interval_tree.h include/comun.h | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Crear directorios si no existen
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Ejecutar el binario (opcional)
run: all
	./$(TARGET)

# Limpiar
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)
