#include <stdio.h>

typedef struct _trampoline_data {
    void (*callback)(struct _trampoline_data *);
    void *parameters;
} trampoline_data;

void trampoline(trampoline_data *data)
{
    while (data->callback != NULL)
        data->callback(data);
}

//-----------------------------------------

typedef struct _factorialParameters {
    int n;
    int product;
} factorialParameters;

void factorial(trampoline_data *data)
{
    factorialParameters *parameters = (factorialParameters *)data->parameters;

    if (parameters->n <= 1) {
        data->callback = NULL;
    } else {
        parameters->product *= parameters->n;
        parameters->n--;
    }
}

int main()
{
    factorialParameters params = { 5, 1 };
    trampoline_data t = { &factorial, &params };
    trampoline(&t);
    printf("\n%d\n", params.product);
    return 0;
}
