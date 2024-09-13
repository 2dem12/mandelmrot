#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "main.hpp"


using namespace std;



class MandelbrotApp {
public:
    MandelbrotApp(double width, double height, double max_iter, char set_name):
        width(width), height(height), max_iter(max_iter),
        x_min(-2.5), x_max(2.5), y_min(-2), y_max(2),
        version(0), set_name(set_name) {

        window.create(sf::VideoMode(width, height), "Mandelbrot Set");
        image.create(width, height);
        draw();
        texture.loadFromImage(image);
        sprite.setTexture(texture);
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            render();
        }
    }

private:
    double width, height;
    char set_name;
    double x_min, x_max, y_min, y_max;
    double max_iter;
    int version;
    vector<pair<double, double>> prev_x;
    vector<pair<double, double>> prev_y;

    sf::RenderWindow window;
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    bool selecting = false;
    sf::Vector2i start_dot, end_dot;

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space && version > 0) {
                    if (prev_x.size() > 0 && prev_y.size() > 0) {
                        x_min = prev_x[--version].first;
                        x_max = prev_x[version].second;
                        y_min = prev_y[version].first;
                        y_max = prev_y[version].second;

                        prev_x.pop_back();
                        prev_y.pop_back();

                        max_iter /= 1.2;
                        draw();
                        texture.loadFromImage(image);
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                start_dot = sf::Mouse::getPosition(window);
                end_dot = start_dot;
                selecting = true;
                // cout << 1 << '\n';
            }

            if (event.type == sf::Event::MouseMoved && selecting) {
                end_dot = sf::Mouse::getPosition(window);
                // cout << 2 << '\n';
            }

            if (event.type == sf::Event::MouseButtonReleased && selecting) {
                selecting = false;
                max_iter *= 1.2;
                // cout << 3 << '\n';
                processSelection();
            }
        }
    }


    void render() {
        // cout << 5 << '\n';
        window.clear();
        window.draw(sprite);

        if (selecting) {
            sf::RectangleShape selectionBox;
            int dx = end_dot.x - start_dot.x;
            int dy = end_dot.y - start_dot.y;
            int size = std::min(abs(dx), abs(dy));

            int x_start = (dx > 0) ? start_dot.x : start_dot.x - size;
            int y_start = (dy > 0) ? start_dot.y : start_dot.y - size;

            selectionBox.setPosition(static_cast<float>(x_start), static_cast<float>(y_start));
            selectionBox.setSize(sf::Vector2f(static_cast<float>(size), static_cast<float>(size)));
            selectionBox.setFillColor(sf::Color(100, 25, 50, 100));
            window.draw(selectionBox);
        }

        window.display();
    }

    void draw() {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                double x = x_min + (x_max - x_min) * i / width;
                double y = y_min + (y_max - y_min) * j / height;
                Complex z(x, y);
                if (set_name == 'j') {
                    x = -0.7;
                    y = 0.27015;
                }

                Complex c(x, y);

                double iter = set_name == 'm' ? mandelbrot(c, max_iter) : julia(z, c, max_iter);
                sf::Color color = getColor(iter, max_iter);
                image.setPixel(i, j, color);
            }
        }
    }

    void processSelection() {
        int dx = end_dot.x - start_dot.x;
        int dy = end_dot.y - start_dot.y;

        int size = std::min(abs(dx), abs(dy));
        int x_start = (dx > 0) ? start_dot.x : start_dot.x - size;
        int y_start = (dy > 0) ? start_dot.y : start_dot.y - size;

        double new_x_min = x_min + (x_max - x_min) * x_start / width;
        double new_x_max = x_min + (x_max - x_min) * (x_start + size) / width;
        double new_y_min = y_min + (y_max - y_min) * y_start / height;
        double new_y_max = y_min + (y_max - y_min) * (y_start + size) / height;

        prev_x.push_back({x_min, x_max});
        prev_y.push_back({y_min, y_max});
        version++;

        x_min = new_x_min;
        x_max = new_x_max;
        y_min = new_y_min;
        y_max = new_y_max;

        draw();
        texture.loadFromImage(image);

    }
};

int main() {
    MandelbrotApp app(1024, 980, 100, 'j'); // set name m -> mandelbrot
                                                                       // set_name j -> julia
    app.run();
    return 0;
}
