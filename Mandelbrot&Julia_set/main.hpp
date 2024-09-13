
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

double julia(Complex z, const Complex& c, double max_iter) {
    double n = 0;
    while (z.magnitude() <= 2 && n < max_iter) {
        z = z * z + c;
        n++;
    }
    return n;
}

sf::Color getColor(int iter, double max_iter) {
    if (iter >= max_iter*0.9) {
        return sf::Color(0, 0, 0);
    } else {
        double t = (double)iter/(double)max_iter;

        int r = (int)(9*(1-t)*t*t*t*255);
        int g = (int)(15*(1-t)*(1-t)*t*t*255);
        int b =  (int)(8.5*(1-t)*(1-t)*(1-t)*t*255);

        return sf::Color(r, g, b);
    }
}
