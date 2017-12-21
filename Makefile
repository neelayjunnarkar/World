OBJS = objs\main.o objs\esShader.o objs\esUtil.o objs\Model.o objs\Mesh.o objs\FunctionModel.o objs\TestModel.o	objs\logging.o objs\FastNoise.o objs\SphericalFunctionModel.o
CPPCFLAGS = -O2 -std=c++14
CCFLAGS = -O2
LFLAGS = -s WASM=1 -s USE_WEBGL2=1

INCLUDE = -Iinclude

all: $(OBJS)
	em++ $(OBJS) $(LFLAGS) -O2 -o out\main.html --shell-file shell_minimal.html --preload-file shaders\vertex_shader.glessl --preload-file shaders\fragment_shader.glessl

objs\main.o: src\main.cpp objs\Model.o
	em++ -c src\main.cpp -o objs/main.o $(INCLUDE) $(CPPCFLAGS)

objs\esShader.o: include\esShader.c
	emcc -c include\esShader.c -o objs/esShader.o $(INCLUDE) $(CCFLAGS)

objs\esUtil.o: include\esUtil.c
	emcc -c include\esUtil.c -o objs/esUtil.o $(INCLUDE) $(CCFLAGS)

objs\Model.o: src\Model.cpp objs\Mesh.o src\Model.hpp
	em++ -c src\Model.cpp -o objs/Model.o $(INCLUDE) $(CPPCFLAGS)

objs\Mesh.o: src\Mesh.cpp src\Mesh.hpp
	em++ -c src\Mesh.cpp -o objs/Mesh.o $(INCLUDE) $(CPPCFLAGS)

objs\FunctionModel.o: src\FunctionModel.cpp src\FunctionModel.hpp
	em++ -c src\FunctionModel.cpp -o objs/FunctionModel.o $(INCLUDE) $(CPPCFLAGS)

objs\SphericalFunctionModel.o: src\SphericalFunctionModel.cpp src\SphericalFunctionModel.hpp
	em++ -c src\SphericalFunctionModel.cpp -o objs/SphericalFunctionModel.o $(INCLUDE) $(CPPCFLAGS)
	
objs\TestModel.o: src\TestModel.cpp src\TestModel.hpp
	em++ -c src\TestModel.cpp -o objs/TestModel.o $(INCLUDE) $(CPPCFLAGS)

objs\logging.o: src\logging.cpp src\logging.hpp
	em++ -c src\logging.cpp -o objs/logging.o $(INCLUDE) $(CPPCFLAGS)
	
objs\FastNoise.o: include\FastNoise.cpp
	em++ -c include\FastNoise.cpp -o objs/FastNoise.o $(INCLUDE) $(CPPCFLAGS)

run:
	emrun --no_browser --port 8080 .

clean:
	rm -f $(OBJS)