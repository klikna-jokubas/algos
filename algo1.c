#include <sys/time.h>
#include <stdio.h>
#include <math.h>



int fib1(int n) {
    if (n < 2)
        return n;
    return fib1(n-1) + fib1(n-2);
}

int fib2(int n) {
    int i = 1, j = 0;
    for (int k = 1; k <= n; k++) {
        j = i + j;
        i = j - i;
    }
    return j;
}

int fib3(int n) {
    int i = 1, j = 0, k = 0, h = 1, t;
    while (n > 0) {
        if (n % 2 != 0) {
            t = j * h;
            j = i * h + j * k + t;
            i = i * k + t;
        }
        t = h * h;
        h = 2 * k * h + t;
        k = k * k + t;
        n /= 2;
    }
    return j;
}

double microseconds() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void executeProgram(const int key, int n) {
    double t1 = 0, t2 = 0;
    double x=0, y=0, z=0, t=0;
    switch (key) {
        case 1:
            t1 = microseconds();
            fib1(n);
            t2 = microseconds();
            t = t2-t1;
            x = t / pow(1.1, n);
            y = t / pow(((1 + sqrt(5)) / 2), n);
            z = t / pow(2, n);
            break;
        case 2:
            t1 = microseconds();
            fib2(n);
            t2 = microseconds();
            t = t2-t1;
            x = t / pow(n, 0.8);
            y = t / n;
            z = t / (n * log(n));
            break;
        case 3:
            t1 = microseconds();
            fib3(n);
            t2 = microseconds();
            t = t2-t1;
            x = t / sqrt(log(n));
            y = t / log(n);
            z = t / pow(n, 0.5);
            break;
        default:
            printf("Wrong key\n");
    }

    printf("%12d%15.3f%15.6f%15.6f%15.6f\n", n,t,x,y,z);

}

int main()
{
    executeProgram(1, 32);
    executeProgram(2, 1000000);
    executeProgram(3, 1000000);

}
