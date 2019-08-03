
TARGET=myapp
CC=gcc
INCLUDE=-I../include -I../include/hiredis
CXX=g++
#编译时选项
CFLAGS= -g -Wall -c $(INCLUDE)
CPPFLAGS=-E -D
LDFLAGS=-L../lib -lpthread -lcrypto  -lhiredis
RM=rm -rf


.PHONY:clean 

SRC=$(wildcard *.cpp)#获取cpp文件
#print:
#	echo "$(SRC)"


DEC=$(patsubst %.cpp,%.o,$(SRC))#所有的cpp名字替换到o（最后的参数是替换名字）

$(TARGET):$(DEC)
	$(CXX) $^ $(LDFLAGS) -o $@
#生成app


%.o:%.cpp
	$(CXX) $(CFLAGS) $<
#所有的生成o文件


#这是伪目标，没有依赖叫伪目标，不能用make调用，在后边这样 make clean调用
clean:
	$(RM) *.o
	$(RM) $(TARGET)
#安装
install:
	-sudo cp $(TARGET) /usr/bin
#卸载
distclean:
	-sudo rm /usr/bin/$(TARGET) -rf
