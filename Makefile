load_balancer: load_balancer.cpp load_balancer.h search_model sort_model
	g++ -std=c++11 -c load_balancer.cpp -o load_balancer
presenter: presenter.h presenter.cpp
	g++ -std=c++11 -c presenter.cpp -o presenter
worker: worker.cpp worker.h search_model
	g++ -std=c++11 -c worker.cpp -o worker
search_model: search_model.cpp search_model.h
	g++ -std=c++11 -c search_model.cpp -o search_model
sort_model: sort_model.cpp sort_model.h
	g++ -std=c++11 -c sort_model.cpp -o sort_model
.: load_balancer presenter worker search_model sort_model main.cpp main_worker.cpp
	g++ -std=c++11 worker search_model main_worker.cpp -o main_worker
	g++ -std=c++11 presenter sort_model main_presenter.cpp -o main_presenter
	g++ -std=c++11 search_model sort_model load_balancer presenter worker main.cpp

clean:
	rm load_balancer presenter worker search_model sort_model main_presenter main_worker main_worke *.o
	rm -rf fifos
