all:
	g++ src/*.cpp src/*.h -o homework

clean:
	rm -f homework
	rm -rf new
