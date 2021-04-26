SHELL=/bin/bash

image          := ubuntu-opencv-mhrise-charm-scanner
dockerfile     := Dockerfile

main           := src/main.cpp
lib            := $(wildcard lib/*.cpp)
bin            := ./bin/mhrise-charm-scanner
optimaze       := -O2

.DEFAULT_GOAL := build


build: src/* lib/* .timestamps/$(dockerfile)
	docker run --rm -it -v `pwd`:/app -w /app $(image) bash -c 'time g++ -o $(bin) -Wall -Wextra -std=c++17 $(optimaze) -L/usr/local/lib -I/usr/local/include/opencv4 $(main) $(lib) -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_features2d -lopencv_video -lopencv_videoio -lopencv_img_hash'
	@echo

build-in-container: src/* lib/* .timestamps/$(dockerfile)
	(time g++ -o $(bin) -Wall -Wextra -std=c++17 $(optimaze) -L/usr/local/lib -I. -I/usr/local/include/opencv4 $(main) $(lib) -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_features2d -lopencv_video -lopencv_videoio -lopencv_img_hash)
	@echo


run:
	docker run --rm -it -v `pwd`:/app -w /app $(image) bash -c 'time ($(bin) > charm-list.txt)'
	@echo

run-in-container:
	(time ($(bin) > charm-list.txt))
	@echo


test: .timestamps/$(dockerfile)
	g++ -fdiagnostics-color=always -Wall -Wextra -std=c++17 test/charm.cpp $(lib) -Igoogletest/googletest -Igoogletest/googletest/include -Igoogletest/googlemock -Ioogletest/googlemock/include -lgtest -lpthread
	./a.out


bash:
	docker run --rm -it -v `pwd`:/app $(image) bash


.timestamps/$(dockerfile): $(dockerfile) src/
	docker build -f $(dockerfile) -t $(image) .
	mkdir -p $$(dirname .timestamps/$(dockerfile))
	touch .timestamps/$(dockerfile)


clean:
	# docker rmi -f $(image)
	rm -f $(deb-file)

