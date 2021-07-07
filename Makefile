# executable name
EXEC=opengl

LIBS=-lm -L/usr/X11R6/lib -lglfw -lglew
CXX=g++
CXXFLAGS=-Wall --std=c++14 -g -MMD -DGL_SILENCE_DEPRECATION
INCLUDES=-I/usr/X11R6/include -framework OpenGL # -framework Cocoa
VENDOR=-Iinclude $(shell pkg-config --cflags glm glfw3 glew)

SRC_DIR=src
BUILD_DIR=build
VENDOR_DIR=include

SRC_FILES=$(shell echo ${SRC_DIR}/*.cc ${SRC_DIR}/**/*.cc)
OBJ_FILES=$(SRC_FILES:${SRC_DIR}%.cc=${BUILD_DIR}%.o)
SRC_DEPS=${OBJ_FILES:.o=.d}

# we expect and assume that the vendor source code to be organized into
# subdirectories within the VENDOR_DIR directory;
VENDOR_SRC=$(shell echo ${VENDOR_DIR}/**/*.cc)
VENDOR_OBJ=$(VENDOR_SRC:${VENDOR_DIR}%.cc=${BUILD_DIR}%.o)

${EXEC}: ${OBJ_FILES} ${VENDOR_OBJ}
	@echo "==========================="
	@echo "linking object files..."
	@mkdir -p ${BUILD_DIR}
	${CXX} -o "${BUILD_DIR}/${EXEC}" ${OBJ_FILES} ${VENDOR_OBJ} ${LIBS} ${INCLUDES}
	@echo "done"

${BUILD_DIR}/%.o: ${SRC_DIR}/%.cc Makefile
	@echo "==========================="
	@echo "compiling project source file '$<'"
	@mkdir -p "$(@D)"
	${CXX} -c $< -o $@ ${CXXFLAGS} ${VENDOR}
	@echo "done"

${BUILD_DIR}/%.o: ${VENDOR_DIR}/%.cc Makefile
	@echo "==========================="
	@echo "compiling vendor source file '$<'"
	@mkdir -p "$(@D)"
	${CXX} -c $< -o $@ ${CXXFLAGS}
	@echo "done"

-include ${SRC_DEPS}

.PHONY: clean

clean:
	rm -rf ${BUILD_DIR}
