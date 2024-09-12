#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>

using namespace std;

struct Complex {
    double real;
    double imag;

    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    Complex operator*(const Complex& other) const {
        return Complex(real * other.real - imag * other.imag, real * other.imag + imag * other.real);
    }

    double magnitude() const {
        return sqrt(real * real + imag * imag);
    }
};



double mandelbrot(const Complex& c, double max_iter) {
    Complex z(0.0, 0.0);
    double n = 0;
    while (z.magnitude() <= 2 && n < max_iter) {
        z = z * z + c;
        n++;
    }
    return n;
}

//
// sf::Vertex draw(double width, double height, double x_min, double x_max,
//      double y_min, double y_max, double max_iter) {
//      for (int i = 0; i < width; i++) {
//          for (int j = 0; j < height; j++) {
//              double x = x_min + (x_max - x_min) * i / width;
//              double y = y_min + (y_max - y_min) * j / height;
//              Complex c(x, y);
//
//              double iter = mandelbrot(c, max_iter);
//
//              sf::Color color;
//              if (iter == max_iter) {
//                  color = sf::Color(0, 0, 0);
//
//              } else {
//                  double clr = 255.0 * iter / max_iter;
//                  color = sf::Color(100, clr, 255 - clr);
//              }
//
//              sf::Vertex point(sf::Vector2f(i, j), color);
//
//              // window.draw(&point, 1, sf::Points);
//              return point;
//          }
//      }
// }


signed main() {
    double height = 980, width = 1024;
    double x_min = -2.5, x_max = 1.5;
    double y_min = -2, y_max = 2;
    double max_iter = 60;
    double moveSpeed = 30.0f;
    int version = 0;
    vector<pair<double, double>> prev_x{pair{x_min, x_max}};
    vector<pair<double, double>> prev_y{pair{y_min, y_max}};
    sf::Texture texture;
    sf::Image image;
    texture.loadFromImage(image);
    sf::Sprite sprite(texture);

    bool selecting = false;
    sf::Vector2i startPos, endPos;

    sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot Set");
    sf::View view = window.getDefaultView();

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }



        // if (event.type == sf::Event::KeyPressed) {
        //     if (event.type == sf::Event::KeyPressed) {
        //         if (event.key.code == sf::Keyboard::Enter and version > 0) {
        //             x_min = prev_x[version].first;
        //             x_max = prev_x[version].second;
        //             y_min = prev_y[version].first;
        //             y_max = prev_y[version--].second;
        //             prev_x.erase(prev_x.end());
        //             prev_y.erase(prev_y.end());
        //
        //             sf::Vertex point = draw(width, height, x_min, x_max, y_min, y_max, max_iter);
        //             window.draw(&point, 1, sf::Points);
        //             texture.loadFromImage(image);
        //         }
        //     }
        //
        //     if (event.type == sf::Event::MouseButtonPressed and event.mouseButton.button == sf::Mouse::Left) {
        //         startPos = sf::Mouse::getPosition(window);
        //         endPos = startPos;
        //         selecting = true;
        //     }
        //
        //     if (event.type == sf::Event::MouseMoved and selecting) {
        //         endPos = sf::Mouse::getPosition(window);
        //     }
        //
        //     if (event.type == sf::Event::MouseButtonReleased and selecting) {
        //         selecting = false;
        //         // Вычисление границ выделенной области с сохранением квадратной формы
        //         int size = std::min(endPos.x - startPos.x, endPos.y - startPos.y);
        //         int x_start = std::min(startPos.x, endPos.x);
        //         int y_start = std::min(startPos.y, endPos.y);
        //
        //         // Преобразование координат в числа Мандельброта
        //         double new_x_min = x_min + (x_max - x_min) * x_start / width;
        //         double new_x_max = x_min + (x_max - x_min) * (x_start + size) / width;
        //         double new_y_min = y_min + (y_max - y_min) * y_start / width;
        //         double new_y_max = y_min + (y_max - y_min) * (y_start + size) / width;
        //         // Сохранение предыдущих границ
        //         prev_x.push_back({x_min, x_max});
        //         prev_y.push_back({y_min, y_max});
        //         version++;
        //
        //         // Обновление текущих границ
        //         x_min = new_x_min;
        //         x_max = new_x_max;
        //         y_min = new_y_min;
        //         y_max = new_y_max;
        //
        //         sf::Vertex point = draw(width, height, x_min, x_max, y_min, y_max, max_iter);
        //         window.draw(&point, 1, sf::Points);
        //         // texture.loadFromImage(image);
        //     }
        // }
        window.setView(view);
        window.clear();



        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                double x = x_min + (x_max - x_min) * i / width;
                double y = y_min + (y_max - y_min) * j / height;
                Complex c(x, y);

                double iter = mandelbrot(c, max_iter);

                sf::Color color;
                if (iter == max_iter) {
                    color = sf::Color(0, 0, 0);

                } else {
                    double clr = 255.0 * iter / max_iter;
                    color = sf::Color(100, clr, 255 - clr);
                }

                sf::Vertex point(sf::Vector2f(i, j), color);

                window.draw(&point, 1, sf::Points);
            }
        }



        window.display();
    }

    return 0;
}
