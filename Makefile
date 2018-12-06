
CC:= g++
CFLAGS= -g -Wall -std=c++11
INC:= -I./src -I./test

BUILD_DIR:= ./build
OBJECTS_DIR:= $(BUILD_DIR)/objects
SRC_DIR:= ./src
TEST_DIR:= ./test

# 静态链接库
LDFLAGS = 
LDLIBS = -lpthread

SRCS:= ./test/testMain.cpp \
	./src/fifo.cpp
# SRCS:= $(widlcard *.cpp)
TARGET:= testMain
OBJECTS:= $(SRCS:.cpp=.o)
# OBJECTS:= $(addprefix $(OBJECTS_DIR)/, $(notdir $(SRCS:.cpp=.o)))

$(TARGET): $(OBJECTS)
	@echo "Building $(TARGET)"
	@$(CC) $^ -o $@

#这段是make的模式规则，指示如何由.c文件生成.o，即对每个.c文件，调用gcc -c XX.c -o XX.o命令生成对应的.o文件。  
#如果不写这段也可以，因为make的隐含规则可以起到同样的效果  
%.o: %.cpp
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@


#这条sed命令的结构是s/match/replace/g。有时为了清晰，可以把每个/写成逗号，即这里的格式s,match,replace,g。
# 该命令表示把源串内的match都替换成replace，s指示match可以是正则表达式。
# g表示把每行内所有match都替换，如果去掉g，则只有每行的第1处match被替换(实际上不需要g，因为一个.d文件中，只会在开头有一个main.o:)。
# 这里match是正则式\($*\)\.o[ :]*，它分成3段：
# 第1段是$*，这里指除了后缀名以外的部分，在sed命令里把$*用和括起来，使接下来的replace中可以用\1引用$*。
# 第2段是\.o，表示匹配$*.o，(这里\不知何意，去掉也是可以的)。
# 第3段是正则式[ :]*，表示若干个空格或冒号，(其实一个.d里只会有一个冒号，如果这里写成[ ]*:，即匹配若干个空格后跟一个冒号，也是可以的)。
# 总体来说match用来匹配‘main.o :‘这样的串。
# 这里的replace是$*.o $@ :，其中\1会被替换为前面第1个和括起的内容，即$*，这样replace值为$*.o $@.d :
# 这样该sed命令就实现了把$*.o :替换为$*.o $@.d :的目的。
# < $@.$$$$，指示sed命令从临时文件$@.$$$$读取输入，作为命令的来源字符串。
# > $@，把行内替换结果输出到最终文件$@。
# %.d:%.cpp
# 	set -e;\
# rm -f $@;\
# echo "111111";\
# $(CC) -MM $(CFLAGS) $< > $@.$$$$;\
# echo "222222";\
# sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@;\
# rm -f $@.$$$$
%.d: %.cpp
	@$(CC) $(CFLAGS) $(INC) -MM -MT $*.d -MT $*.o "$<" > $*.d

# $(CC) $(CFLAGS) -MM -MG -MP -MF $*.d -MT $*.d -MT $*.o -MP "$<"
# 第一次make，假定这时从来没有make过，所有.d文件不存在，这时键入make：
# 1-include所有.d文件的命令无效果。
# 2-首次编译所有.c文件。每个.c文件中若#include了其它头文件，会由编译器自动读取。由于这次是完整编译，不存在什么依赖文件改了不会重编的问题。
# 3-对每个.c文件，会根据依赖规则%.d: %.c，生成其对应的.d文件
-include $(OBJECTS:.o=.d)

.phony: clean

clean:
	@rm -rf $(TARGET) $(OBJECTS) $(OBJECTS:.o=.d)
# rm -rf $(BUILD_DIR)