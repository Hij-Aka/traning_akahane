CC = gcc
CXX = g++
CFLAGS = 
CXXFLAGS= -std=c++11 $(CFLAGS)

SIMUDIR = ../simulator
SRCS_SIMU = robot.cpp simulator.cpp
OBJS_SIMU = $(addprefix $(SIMUDIR)/, $(addsuffix .o, $(basename $(SRCS_SIMU))))

SRCS_VISU = visualize.cpp main.cpp 
OBJS_VISU = $(addsuffix .o, $(basename $(SRCS_VISU)))
OBJS = $(OBJS_VISU) $(OBJS_SIMU)

LIBS += -lglut -lGLU -lGL -lm -pthread -I ../eigen3/Eigen/
TARGET = visualize

$(TARGET): $(OBJS)
	$(CXX) $^ $(LIBS) -o $@

$(SIMUDIR)/%.o: $(SIMUDIR)/%.cpp
	@[ -d $(OBJDIR) ]
	$(CXX) -o $@ -c $<

.PHONY: clean

clean:
	rm -rf *.o $(TARGET) optimize