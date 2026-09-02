CXX = g++
FLGS = -O3 -flto
DEF = -DGLM_FORCE_INTRINSICS -DGLM_FORCE_DEFAULT_ALIGNED_GENTYPES -DGLM_ENABLE_EXPERIMENTAL
LIBS = -lglfw -I./include
SRC = main.cpp src/engine/*.cpp include/glad/glad.c

a.elf: main.cpp
	$(CXX) $(FLGS) $(DEF) $(SRC) -o a.elf $(LIBS)

.PHONY = ctdbg

ctdbg: main.cpp
	$(CXX) $(FLGS) $(DEF) $(SRC) -o a.elf $(LIBS)
	./a.elf
	rm a.elf
