__generate:
	cmake -S . -B build

__build:
	cmake --build build

__execute:
	./build/trial_app

run: __generate __build __execute