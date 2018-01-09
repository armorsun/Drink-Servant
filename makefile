CC=g++
CFLAGS=`pkg-config --cflags --libs opencv` -std=c++11 -DDEBUG=0
OBJS=send_to_arduino.o main.o make_beverage.o catch_cup.o
TARGET=drink_service
all: main

main: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)

remote_control: remote_control_arm_record_video.o send_to_arduino.o
	$(CC) -c $^ -o remote_control $(CFLAGS)

move_car: move_car.cpp send_to_arduino.cpp
	$(CC) -c $^ -o $@ -std=c++11

main.o: main.cpp
	$(CC) -c $< -o $@ -std=c++11

remote_control_arm_record_video.o: remote_control_arm_record_video.cpp 
	$(CC) -c $< -o $@ -std=c++11

send_to_arduino.o: send_to_arduino.cpp send_to_arduino.h
	$(CC) -c $^ -o $@ -std=c++11

make_beverage.o: make_beverage.cpp make_beverage.h
	$(CC) -c $^ -o $@ -std=c++11

catch_cup.o: catch_cup.cpp catch_cup.h
	 $(CC) -c $^ -o $@ $(CFLAGS)

clean: 
	rm $(TARGET) $(OBJS)
