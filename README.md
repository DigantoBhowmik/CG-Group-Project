```
g++ -std=c++17 -pthread -o opengl101 main.cpp -lglut -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl -Ofast && ./opengl101
npx nodemon -w "**/.h" -w "**/.cpp" --exec "g++ main.cpp -std=c++17 -pthread -o opengl101 -Ofast -framework OpenGL -framework GLUT && ./opengl101"
```
