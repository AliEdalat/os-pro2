load_balancer: load_balancer.cpp load_balancer.h
	g++ -c load_balancer.cpp -o load_balancer
presenter: presenter.h presenter.cpp
	g++ -c presenter.cpp -o presenter
worker: worker.cpp worker.h
	g++ -c worker.cpp -o worker
.: load_balancer presenter worker
	g++ load_balancer presenter worker main.cpp

clean:
	rm load_balancer presenter worker 