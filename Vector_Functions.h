struct Vector{
    double x;
    double y;
};

double length(Vector A);

double dot(Vector A, Vector B);

Vector add(Vector A, Vector B);

Vector subtract(Vector A, Vector B);

Vector multiply(Vector A, double C);

Vector direction(Vector A, Vector B);