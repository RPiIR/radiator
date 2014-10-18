
all: test_light test_display

test_light: test_light.c
	gcc -o test_light test_light.c info_rad.c -lwiringPi

test_display: test_display.c
	gcc -o test_display test_display.c info_rad.c -lwiringPi


