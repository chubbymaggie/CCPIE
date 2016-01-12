pie: src/pie.cpp include/formatter.hpp include/learner.hpp include/pie.hpp \
     include/types.hpp include/features.hpp
	$(CXX) -std=c++14 -O3 -I include/ src/pie.cpp -o bin/pie

clean:
	rm -rf pie
