#include <stdio.h>
#include "ML_Assign_1.h"

int main(int argc, char const *argv[]) {
    float KPMatrix[10][5] = {0};
    int r = 3;

    for (int k = 0; k < 10; k++) {
        for (int p = 0; p < 5; p++) {
            KPMatrix[k][p] = KNNSpecific("data.txt", k+1, p+1, r, 210, 7);
            printf("%f ", KPMatrix[k][p]);
        }
        printf("\n");
    }

    int max_k = 0;
    int max_p = 0;
    float max_acc = KPMatrix[0][0];

    for (int k = 0; k < 10; k++) {
        for (int p = 0; p < 5; p++) {
            if (max_acc < KPMatrix[k][p]) {
                max_acc = KPMatrix[k][p];
                max_k = k;
                max_p = p;
            }
        }
    }

    printf("\n\nThe value of K = %d\nThe value of P = %d\nThe max accuracy is %f\n", max_k+1, max_p+1, KPMatrix[max_k][max_p]);

    return 0;
}
