#include <math.h>
#include <omp.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define BLOCK_SIZE 20

typedef struct launch {
    int threads_count; // Count of used threads
    double eps; // Epsilon
    size_t net_size; // Net size, points count N x N
} launch_t;

typedef struct net {
    size_t size; // Net size
    double delta; // Side length of one block

    double **u;
    double **f;
} net_t;

typedef struct results {
    double t;
    size_t iter;
} results_t;

typedef double (*fun_xy)(double, double);

launch_t parse_argumnets(char **argv) {
    launch_t launch;

    launch.threads_count = strtol(argv[1], NULL, 10);
    launch.eps = strtod(argv[2], NULL);
    launch.net_size = strtoul(argv[3], NULL, 10);

    return launch;
}

double **create_2d_array(size_t sz) {
    double **res = calloc(sz, sizeof(*res));
    for (size_t i = 0; i < sz; i++)
        res[i] = calloc(sz, sizeof(*res[i]));
    return res;
}

net_t *create_net(size_t sz, fun_xy f, fun_xy u) {
    net_t *res = calloc(1, sizeof(*res));
    res->size = sz;
    res->delta = 1.0 / (sz - 1);
    res->u = create_2d_array(sz);
    res->f = create_2d_array(sz);

    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz; j++) {
            if ((i == 0) || (j == 0) || (i == (sz - 1)) || (j == (sz - 1))) {
                res->u[i][j] = u(i * res->delta, j * res->delta);
            } else {
                res->u[i][j] = 0;
            }
            res->f[i][j] = f(i * res->delta, j * res->delta);
        }
    }
    return res;
}
double calculate_block(net_t *nt, int a, int b) {
    int i0 = 1 + a * BLOCK_SIZE;
    int j0 = 1 + b * BLOCK_SIZE;
    int im = MIN(i0 + BLOCK_SIZE, nt->size - 1);
    int jm = MIN(j0 + BLOCK_SIZE, nt->size - 1);

    double dm = 0;
    double temp, d;
    for (int i = i0; i < im; i++) {
        for (int j = j0; j < jm; j++) {
            temp = nt->u[i][j];
            nt->u[i][j] = 0.25 * (nt->u[i - 1][j] + nt->u[i + 1][j] + nt->u[i][j - 1] +
                                  nt->u[i][j + 1] - nt->delta * nt->delta * nt->f[i][j]);
            d = fabs(temp - nt->u[i][j]);
            if (dm < d)
                dm = d;
        }
    }
    return dm;
}

size_t calculate_net(net_t *nt, double eps) {
    size_t iter = 0;
    size_t calc_size = nt->size - 2;
    int blocks_number = (calc_size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    double dmax = 0; // maximum deflection
    double *dm = calloc(blocks_number, sizeof(*dm)); // deflection for each block

    do {
        iter++;
        dmax = 0;
        for (int nx = 0; nx < blocks_number; nx++) {
            dm[nx] = 0;

            int i, j;
            double d;

#pragma omp parallel for shared(nt, nx, dm) private(i, j, d)
            for (i = 0; i < nx + 1; i++) {
                j = nx - i;
                d = calculate_block(nt, i, j);
                if (dm[i] < d)
                    dm[i] = d;
            }
        }

        for (int nx = blocks_number - 2; nx > -1; nx--) {
            int i, j;
            double d;

#pragma omp parallel for shared(nt, nx, dm) private(i, j, d)
            for (i = blocks_number - nx - 1; i < blocks_number; i++) {
                j = 2 * (blocks_number - 1) - nx - i;
                d = calculate_block(nt, i, j);
                if (dm[i] < d)
                    dm[i] = d;
            }
        }

        for (int i = 0; i < blocks_number; i++)
            if (dmax < dm[i])
                dmax = dm[i];
    } while (dmax > eps);
    free(dm);

    return iter;
}

results_t run_test(launch_t launch, fun_xy f, fun_xy u) {
    omp_set_num_threads(launch.threads_count);
    net_t *net = create_net(launch.net_size, f, u);

    double t1, t2, dt;
    t1 = omp_get_wtime();
    size_t iter = calculate_net(net, launch.eps);
    t2 = omp_get_wtime();
    dt = t2 - t1;

    net_t *dif_net = create_net(launch.net_size, u, u);

    double max = -INFINITY;
    double sum = 0;

    for (size_t i = 0; i < net->size; i++) {
        for (size_t j = 0; j < net->size; j++) {
            dif_net->u[i][j] = dif_net->f[i][j] - net->u[i][j];
            sum += fabs(dif_net->u[i][j]);
            max = fmax(max, dif_net->u[i][j]);
        }
    }

    printf("Max: %f \n", max);
    printf("Average: %f \n", sum / (net->size * net->size));

    results_t results = {dt, iter};

    return results;
}

double d_kx3_p_2ky3(double x, double y) { return 6000 * x + 12000 * y; }

double kx3_p_2ky3(double x, double y) { return 1000 * pow(x, 3) + 2000 * pow(y, 3); }

int main(int argc, char **argv) {
    // parse command line arguments
    if (argc != 4) {
        return 1;
    }
    fun_xy f = d_kx3_p_2ky3; // d_book;
    fun_xy u = kx3_p_2ky3; // book;
    launch_t launch = parse_argumnets(argv);
    // printf("%i %zu %i\n", launch.block_size, launch.net_size, launch.threads_count);
    results_t results = run_test(launch, f, u);
    printf("Iterations: %zu\nTime: %f\n", results.iter, results.t);
    return 0;
}
