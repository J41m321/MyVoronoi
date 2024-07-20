#include <SFML/Graphics.hpp> //Para gráficos y ventanas
#include <boost/polygon/voronoi.hpp> //Para diagramas de Voronoi
#include <vector> 
#include <iostream> 

using namespace boost::polygon; 
using namespace std; 

// Definiciones de tipos 
typedef voronoi_diagram<double>::cell_type cell_type;
typedef voronoi_diagram<double>::vertex_type vertex_type;
typedef voronoi_diagram<double>::edge_type edge_type;
typedef point_data<int> point;

class InteractiveVoronoi {
public:
    // Constructor: inicializa la ventana de SFML
    InteractiveVoronoi() : window(sf::VideoMode(800, 600), "Diagrama de Voronoi Interactivo") {
        window.setFramerateLimit(60); // Limita la velocidad de fotogramas a 60 FPS (recomendado)
    }

    // Ejecuta el bucle de la aplicación
    void run() {
        while (window.isOpen()) {
            handleEvents(); 
            render(); 
        }
    }

private:
    sf::RenderWindow window; // Ventana de SFML para mostrar la interfaz gráfica
    vector<point> points; // Vector que almacena los sitios añadidos
    voronoi_diagram<double> vd; 

    // Maneja los eventos de la ventana
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) { //procesamiento de eventos en la cola
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) { 
                    // Añade el punto a la lista de puntos y construye el diagrama de Voronoi
                    points.emplace_back(event.mouseButton.x, event.mouseButton.y);
                    constructVoronoi();
                }
            }
        }
    }

    // Construye el diagrama de Voronoi a partir de los puntos actuales
    void constructVoronoi() {
        vd.clear(); // Limpia el diagrama de Voronoi anterior (recalcula todo nuevamente)
        // Construye el nuevo diagrama de Voronoi con los puntos actuales
        construct_voronoi(points.begin(), points.end(), &vd); //función de Boost
    }

    // Renderiza la ventana de SFML
    void render() {
        window.clear(sf::Color::White); 

        // Dibuja todos los puntos (sitios) en la ventana
        for (const auto& point : points) {
            sf::CircleShape shape(3); 
            shape.setFillColor(sf::Color::Blue); 
            shape.setPosition(point.x() - 3, point.y() - 3);
            window.draw(shape); 
        }

        // Dibuja todas las aristas del diagrama de Voronoi
        for (const auto& edge : vd.edges()) {
            if (edge.is_infinite()) continue; //se ignoran las infinitas

            const vertex_type* v0 = edge.vertex0(); 
            const vertex_type* v1 = edge.vertex1(); 

            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(v0->x(), v0->y()), sf::Color::Red),
                sf::Vertex(sf::Vector2f(v1->x(), v1->y()), sf::Color::Red) 
            };

            window.draw(line, 2, sf::Lines); 
        }

        window.display(); 
    }
};

int main() {
    InteractiveVoronoi iv; 
    iv.run(); 
    return 0; 
}