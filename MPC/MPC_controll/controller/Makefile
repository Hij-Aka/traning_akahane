CC = gcc
CXX = g++
CFLAGS = -O3 -g 
CXXFLAGS= -std=c++17 $(CFLAGS)
 
SIMUDIR = ../simulator
SRCS_SIMU = robot.cpp simulator.cpp
OBJS_SIMU = $(addprefix $(SIMUDIR)/, $(addsuffix .o, $(basename $(SRCS_SIMU))))
 
OPTIDIR = ../optimizer
SRCS_OPTI = optimize.cpp
OBJS_OPTI = $(addprefix $(OPTIDIR)/, $(addsuffix .o, $(basename $(SRCS_OPTI))))
 
# SRCS_CONT = main.cpp control.cpp serial.cpp serial_stream.cpp
SRCS_CONT = main.cpp control.cpp serial.cpp 
OBJS_CONT = $(addsuffix .o, $(basename $(SRCS_CONT)))
 
OBJS = $(OBJS_CONT) $(OBJS_OPTI) $(OBJS_SIMU)
 
#LIBS += -lglut -lGLU -lGL -lm -pthread -lboost_iostreams -I ../eigen3/Eigen/
LIBS += -lglut -lGLU -lGL -lm -pthread -I ../eigen3/Eigen/
TARGET = control
 
$(TARGET): $(OBJS)
	$(CXX) $^ $(LIBS) -o $@
 
$(SIMUDIR)/%.o: $(SIMUDIR)/%.cpp
	@[ -d $(OBJDIR) ]
	$(CXX) -o $@ -c $<
 
$(OPTIDIR)/%.o: $(OPTIDIR)/%.cpp
	@[ -d $(OBJDIR) ]
	$(CXX) -o $@ -c $<
 
$(SERIDIR)/%.o: $(SERIDIR)/%.cpp
	@[ -d $(OBJDIR) ]
	$(CXX) -o $@ -c $<
 
.PHONY: clean
 
clean:
	rm -rf *.o $(TARGET) control
 
 
 
# CC = gcc
# CXX = g++
# CFLAGS = -O -g 
# CXXFLAGS= -std=c++11 $(CFLAGS)
# SRCS = serial.cpp
# SRCSDIR = .
# OBJDIR = .
# OBJS = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(basename $(SRCS))))
# # LIBS += -lglut -lGLU -lGL -lm -pthread
# TARGET = run
# $(TARGET): $(OBJS)
# 	$(CXX) $^ $(LIBS) -o $@
 
# $(OBJDIR)/%.o: $(SRCSDIR)/%.cpp
# 	@[ -d $(OBJDIR) ]
# 	$(CXX) -o $@ -c $<
 
# .PHONY: clean
# #.PHONYはタスクターゲットを宣言するためのターゲット
 
# clean:
# 	rm -rf ./*.o $(OBJDIR)/*.o $(TARGET)