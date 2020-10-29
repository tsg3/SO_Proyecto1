COMPILE = `pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs --cflags`

planner: 
	gcc planner.c -o bin/planner -lpthread $(COMPILE)

run_planner:
	./bin/planner

gui:
	gcc main.c -o bin/main -lpthread $(COMPILE)

run_gui:
	./bin/main