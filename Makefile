OBJECTS = sound.o decode_mp3.o

PWD = `pwd`


.PHONY:all
all: wsound libwsound.so

wsound: $(OBJECTS) 
	gcc -o wsound $(OBJECTS) \
    -L ./depends/ao/lib/ \
    -L ./depends/mpg123/lib/ \
    -Wl,-rpath,$(PWD)/depends/ao/lib/,-rpath,$(PWD)/depends/mpg123/lib/ \
    -lao -lmpg123

libwsound.so: $(OBJECTS)
	gcc -o libwsound.so $(OBJECTS) \
    -DSHAREOBJ -fPIC -shared \
    -L ./depends/ao/lib/ \
    -L ./depends/mpg123/lib/ \
    -Wl,-rpath,$(PWD)/depends/ao/lib/,-rpath,$(PWD)/depends/mpg123/lib/ \
    -lao -lmpg123


sound.o: sound.c sound.h
	gcc -c sound.c \
    -I ./depends/ao/include/ -Wall

decode_mp3.o: decode_mp3.c sound.h
	gcc -c decode_mp3.c \
    -I ./depends/mpg123/include/ \
    -I ./depends/ao/include/ \
    -Wall


.PHONY:clean
clean:
	-rm -f wsound libwsound.so $(OBJECTS)
