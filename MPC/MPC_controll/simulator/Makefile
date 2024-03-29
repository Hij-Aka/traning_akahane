CC = gcc
CXX = g++
CFLAGS = -O3
CXXFLAGS= -std=c++11 $(CFLAGS)
SRCS = robot.cpp main.cpp simulator.cpp
OBJS = $(addsuffix .o, $(basename $(SRCS)))

SRCS_OPTI = robot.cpp optimize.cpp simulator.cpp
OBJS_OPTI = $(addsuffix .o, $(basename $(SRCS_OPTI)))

LIBS += -lglut -lGLU -lGL -lm -pthread -I ../eigen3/Eigen/
TARGET = simu
$(TARGET): $(OBJS)
	$(CXX) $^ $(LIBS) -o $@

optimize:$(OBJS_OPTI)
	$(CXX) $^ $(LIBS) -o $@

.PHONY: clean

clean:
	rm -rf *.o $(TARGET) optimize