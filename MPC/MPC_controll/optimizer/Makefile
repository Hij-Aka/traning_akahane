CC = gcc
CXX = g++
CFLAGS = -O3 -g
CXXFLAGS= -std=c++11 $(CFLAGS)
SRCS = robot.cpp main.cpp simulator.cpp
SRCS = $(addprefix ../simulator/, $(SRCS_FILES))

SIMUDIR = ../simulator
SRCS_SIMU = robot.cpp simulator.cpp
OBJS_SIMU = $(addprefix $(SIMUDIR)/, $(addsuffix .o, $(basename $(SRCS_SIMU))))

SRCS_OPTI = optimize.cpp main.cpp
OBJS_OPTI = $(addsuffix .o, $(basename $(SRCS_OPTI)))

OBJS = $(OBJS_OPTI) $(OBJS_SIMU)

LIBS += -lglut -lGLU -lGL -lm -pthread -I ../eigen3/Eigen/
TARGET = optimize

$(TARGET): $(OBJS)
	$(CXX) $^ $(LIBS) -o $@

$(SIMUDIR)/%.o: $(SIMUDIR)/%.cpp
	@[ -d $(OBJDIR) ]
	$(CXX) -o $@ -c $<

.PHONY: clean

clean:
	rm -rf *.o $(TARGET) optimize