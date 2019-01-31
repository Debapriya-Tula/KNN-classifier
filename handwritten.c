#include "ML_Assign_2.h"

int main() {
    /*
    float KPMatrix[10][5] = {0};
    int r = 18;

    for (int k = 0; k < 10; k++) {
        for (int p = 0; p < 5; p++) {
            printf("k = %d, p = %d\n", k+1, p+1);
            KPMatrix[k][p] = KNNSpecific("data.txt", k+1, p+1, r, 3823, 64);
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

    printf("\n\nThe value of K = %d\nThe value of P = %d\nThe min error is %f\n", max_k+1, max_p+1, KPMatrix[max_k][max_p]);
*/
    int k = 1;
    int p = 1;
    int r = 18;
    float metric = KNNSpecific("optdigits.txt", k, p, r, 3823, 64);
    printf("%f\n", metric);
    return 0;
}
