# executable name
EXEC=opengl

LIBS=-lm -L/usr/X11R6/lib -lglfw -lglew
CXX=g++
CFLAGS=-Wall --std=c++14
INCLUDES=-I/usr/X11R6/include -framework OpenGL # -framework Cocoa
VENDOR=-Iinclude

SRC_DIR=src
BUILD_DIR=build
VENDOR_DIR=include

SRC_FILES=$(shell echo ${SRC_DIR}/*.cc ${SRC_DIR}/**/*.cc)
OBJ_FILES=$(SRC_FILES:${SRC_DIR}%.cc=${BUILD_DIR}%.o)

VENOR_SRC=$(shell echo ${VENDOR_DIR}/*.cc ${VENDOR_DIR}/**/*.cc)
VENDOR_OBJ=$(VENOR_SRC:${VENDOR_DIR}%.cc=${BUILD_DIR}%.o)

${EXEC}: ${OBJ_FILES} ${VENDOR_OBJ}
	@echo "==========================="
	@echo "linking object files..."
	@mkdir -p ${BUILD_DIR}
	${CXX} -o "${BUILD_DIR}/${EXEC}" ${OBJ_FILES} ${LIBS} ${INCLUDES}
	@echo "done"

${BUILD_DIR}/%.o: ${SRC_DIR}/%.cc
	@echo "==========================="
	@echo "compiling project source file '$<'"
	@mkdir -p "$(@D)"
	${CXX} -c $< -o $@ ${CFLAGS} ${VENDOR}
	@echo "done"

${BUILD_DIR}/%.o: ${VENDOR_DIR}/%.cc
	@echo "==========================="
	@echo "compiling vendor source file '$<'"
	@mkdir -p "$(@D)"
	${CXX} -c $< -o $@ ${CFLAGS}
	@echo "done"

.PHONY: clean

clean:
	rm -rf ${BUILD_DIR}
