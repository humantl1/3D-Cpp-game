build:
	g++ -w -std=c++14 \
	./src/*.cc \
	-o game \
	-I"./lib/lua" \
	-L"./lib/lua" \
	-llua5.3 \
	-ISDL2 \
	-lSDL2_image \
	-lSDL2_ttf \
	-lSDL2_mixer;

clean:
	rm ./game;

run:
	./game;
