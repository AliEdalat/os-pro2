load_balancer: load_balancer.cpp load_balancer.h search_model sort_model
	g++ -c load_balancer.cpp -o load_balancer
presenter: presenter.h presenter.cpp
	g++ -c presenter.cpp -o presenter
worker: worker.cpp worker.h search_model
	g++ -c worker.cpp -o worker
search_model: search_model.cpp search_model.h
	g++ -c search_model.cpp -o search_model
sort_model: sort_model.cpp sort_model.h
	g++ -c sort_model.cpp -o sort_model
.: load_balancer presenter worker search_model sort_model main.cpp
	g++ search_model sort_model load_balancer presenter worker main.cpp

clean:
	rm load_balancer presenter worker search_model sort_model