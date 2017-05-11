NAME=signdetector
CC=g++
SRC=signdetect.cpp

CFLAGS=
all:
	# $(CC) `pkg-config --cflags --libs opencv3`  $(SRC) -o $(NAME)
	$(CC) -L/usr/local/opt/opencv3/lib -I/usr/local/opt/opencv3/include $(SRC) -o $(NAME) -lopencv_core -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs -lopencv_objdetect -lopencv_features2d -lopencv_highgui

LinuxAll:
	$(CC) -L/usr/local/lib $(SRC) -o $(NAME) -lopencv_core -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs -lopencv_objdetect -lopencv_features2d -lopencv_highgui

clean:
	rm $(NAME)
