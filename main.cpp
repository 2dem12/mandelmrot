#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
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

// Функция для вычисления Мандельброта
double mandelbrot(const Complex& c, double max_iter) {
    Complex z(0.0, 0.0);
    double n = 0;
    while (z.magnitude() <= 2 && n < max_iter) {
        z = z * z + c;
        n++;
    }
    return n;
}

// Получение цвета в зависимости от итерации
sf::Color getColor(int iter, double max_iter) {
    if (iter == max_iter) {
        return sf::Color(0, 0, 0); // Чёрный цвет для точек, входящих в множество Мандельброта
    } else {
        double clr = 255.0 * iter / max_iter;
        return sf::Color(100, clr, 255 - clr); // Градиентный цвет для точек, выходящих за пределы
    }
}


// Отрисовка множества Мандельброта
void draw(sf::Image& image, double width, double height, double x_min, double x_max, double y_min, double y_max, double max_iter) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            double x = x_min + (x_max - x_min) * i / width;
            double y = y_min + (y_max - y_min) * j / height;
            Complex c(x, y);

            double iter = mandelbrot(c, max_iter);
            sf::Color color = getColor(iter, max_iter);
            image.setPixel(i, j, color); // Устанавливаем пиксель с соответствующим цветом
        }
    }
}

int main() {
    double height = 980, width = 1024;
    double x_min = -2.5, x_max = 1.5;
    double y_min = -2, y_max = 2;
    double max_iter = 100;
    int version = 0;
    vector<pair<double, double>> prev_x{pair{x_min, x_max}};
    vector<pair<double, double>> prev_y{pair{y_min, y_max}};

    sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot Set");
    sf::Image image;
    image.create(width, height);
    draw(image, width, height, x_min, x_max, y_min, y_max, max_iter);

    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite(texture);

    bool selecting = false;
    sf::Vector2i start_dot, end_dot;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Обработка нажатия Enter для возврата к предыдущему состоянию
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space && version > 0) {
                    if (prev_x.size() > 1 && prev_y.size() > 1) {
                        x_min = prev_x[--version].first;
                        x_max = prev_x[version].second;
                        y_min = prev_y[version].first;
                        y_max = prev_y[version].second;

                        prev_x.pop_back(); // Удаляем последнее сохраненное состояние
                        prev_y.pop_back(); // Удаляем последнее сохраненное состояние

                        draw(image, width, height, x_min, x_max, y_min, y_max, max_iter);
                        texture.loadFromImage(image);
                    }
                }
            }

            // Обработка начала выделения области мышкой
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                start_dot = sf::Mouse::getPosition(window);
                end_dot = start_dot;
                selecting = true;
            }

            // Обновление позиции окончания выделения при движении мышки
            if (event.type == sf::Event::MouseMoved && selecting) {
                end_dot = sf::Mouse::getPosition(window);
            }

            // Обработка завершения выделения области мышкой
            if (event.type == sf::Event::MouseButtonReleased && selecting) {
                selecting = false;
                // Вычисление размеров выделенной области и сохранение квадратной формы
                int dx = end_dot.x - start_dot.x;
                int dy = end_dot.y - start_dot.y;

                // Убедимся, что размер выделенной области всегда положительный
                int size = std::min(abs(dx), abs(dy));
                int x_start;
                int y_start;

                // Вычисляем координату x_start
                if (dx > 0) {
                    x_start = start_dot.x; // Если dx положительное, начальная точка x не изменяется
                } else {
                    x_start = start_dot.x - size; // Если dx отрицательное, сдвигаем начальную точку x влево на размер выделения
                }

                // Вычисляем координату y_start
                if (dy > 0) {
                    y_start = start_dot.y; // Если dy положительное, начальная точка y не изменяется
                } else {
                    y_start = start_dot.y - size; // Если dy отрицательное, сдвигаем начальную точку y вверх на размер выделения
                }


                // Преобразование координат экрана в координаты множества Мандельброта
                double new_x_min = x_min + (x_max - x_min) * x_start / width;
                double new_x_max = x_min + (x_max - x_min) * (x_start + size) / width;
                double new_y_min = y_min + (y_max - y_min) * y_start / height;
                double new_y_max = y_min + (y_max - y_min) * (y_start + size) / height;

                // Сохранение предыдущих границ
                prev_x.push_back({x_min, x_max});
                prev_y.push_back({y_min, y_max});
                version++;

                // Обновление текущих границ
                x_min = new_x_min;
                x_max = new_x_max;
                y_min = new_y_min;
                y_max = new_y_max;

                draw(image, width, height, x_min, x_max, y_min, y_max, max_iter);
                texture.loadFromImage(image);
            }
        }

        window.clear();
        window.draw(sprite);

        // Отрисовка выделенной области (если она есть)
        if (selecting) {
            sf::RectangleShape selectionBox;
            int dx = end_dot.x - start_dot.x;
            int dy = end_dot.y - start_dot.y;
            int size = std::min(abs(dx), abs(dy));

            int x_start;
            int y_start;

            // Вычисляем координату x_start
            if (dx > 0) {
                x_start = start_dot.x; // Если dx положительное, начальная точка x не изменяется
            } else {
                x_start = start_dot.x - size; // Если dx отрицательное, сдвигаем начальную точку x влево на размер выделения
            }

            // Вычисляем координату y_start
            if (dy > 0) {
                y_start = start_dot.y; // Если dy положительное, начальная точка y не изменяется
            } else {
                y_start = start_dot.y - size; // Если dy отрицательное, сдвигаем начальную точку y вверх на размер выделения
            }


            selectionBox.setPosition(static_cast<float>(x_start), static_cast<float>(y_start));
            selectionBox.setSize(sf::Vector2f(static_cast<float>(size), static_cast<float>(size)));
            selectionBox.setFillColor(sf::Color(255, 0, 0, 100)); // Полупрозрачный красный квадрат
            window.draw(selectionBox);
        }

        window.display();
    }

    return 0;
}
