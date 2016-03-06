all:
	g++ -g -o0 src/*.cpp src/*.h -o homework

clean:
	rm -f homework
	rm -f 0
	rm -rf new
