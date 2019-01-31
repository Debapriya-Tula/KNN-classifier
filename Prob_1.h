#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


int float_compare(const void* elem1, const void* elem2)
{
    if(*(const float*)elem1 < *(const float*)elem2)
        return -1;
    return *(const float*)elem1 > *(const float*)elem2;
}

int rev_int_compare(const void* elem1, const void* elem2)
{
    if(*(const int*)elem1 > *(const int*)elem2)
        return -1;
    else
    if(*(const int*)elem1 < *(const int*)elem2)
        return 1;
}

void print_array(int m,int n,float arr[m][n])
{
    int i,j;
    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            printf("%f ", arr[i][j]);
        }
        printf("\n");
    }
}


float minkowskiDist(float x[], float y[], int order, int num_elements) {
    float sum = 0;
    for (int i = 0; i < num_elements; i++) {
        float temp = pow(fabs(x[i] - y[i]), order);
        sum += temp;
    }

    return pow(sum, 1.0 / order);
}

int count_classes(int block_size, int n_examples, float arr[n_examples-block_size][2],int k,int no_of_classes,int count_class[])
{
    for(int i = 0; i < k; i++)
        count_class[(int)(arr[i][1]) -1] += 1;

    //qsort(count_class, no_of_classes, sizeof(int), rev_int_compare);

    int maxPos = 0;
    int max = count_class[0];
    for (int i = 0; i < no_of_classes; i++) {
        if (count_class[i] > max) {
            max = count_class[i];
            maxPos = i;
        }
    }

    return maxPos + 1;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

float KNNSpecific(char* filename, int n_neighbors, int minkowskiOrder, int n_blocks, int n_examples, int n_attributes) {
    srand((unsigned int) time(NULL)); // seed for generating random numbers
    FILE *ptr = fopen(filename, "r"); // open datasets file
    char buff[1000];
    char *token;
    float seeds_dataset[n_examples][n_attributes+1];
    float seeds_dataset_shuffled[n_examples][n_attributes+1];

    int i=0,j;
    while(fgets(buff,1000,ptr))
    {
        j=0;
        token = strtok(buff, "\t\n,");
        while (token != NULL && j <= n_attributes + 1)
        {
            seeds_dataset[i][j] = atof(token);
            token = strtok(NULL, "\t\n,");
            j++;
        }
        i++;
    }

    int indices[n_examples];
    int dest_index = 0;
    for (int i = 0; i < n_examples; i++) {
        indices[i] = i;
    } // populate indices array

    for (int i = 0; i < n_examples; i++) {
        dest_index = rand() % n_examples;
        while(dest_index == i) {
            dest_index = rand() % n_examples;
        }

        //swap current index with dest_index
        swap(&indices[i], &indices[dest_index]);
    } // shuffle the indices array

    //shuffle the dataset
    for (int i = 0; i < n_examples; i++) {
    	for (int j = 0; j < n_attributes + 1; j++) {
    		seeds_dataset_shuffled[i][j] = seeds_dataset[indices[i]][j];
    	}
    }


    int block_size = n_examples / n_blocks;
    float validation_block[block_size][n_attributes + 1];
    int count = 0;
    float dist_array[n_examples - block_size][2];
    int misclassified = 0;
    int classified = 0;

    for (int block_num = 0; block_num < n_blocks; block_num++) {
        for (int i = block_num * block_size; i < (block_num + 1) * block_size; i++) {
            for (int j = 0; j < n_attributes + 1; j++) {
                validation_block[i - (block_num * block_size)][j] = seeds_dataset_shuffled[i][j];
            }

            //printf("New Point in validation block\n");

            int new_index = 0;
            float total_dist = 0;

            for (int x = 0; x < block_num * block_size; x++) {
                total_dist = minkowskiDist(validation_block[i - (block_num * block_size)], seeds_dataset_shuffled[x], minkowskiOrder, n_attributes);
                dist_array[new_index][0] = total_dist;
                dist_array[new_index][1] = seeds_dataset_shuffled[x][n_attributes];
                new_index++;
                //printf("First block %f",seeds_dataset_shuffled[x][7]);
            }

            for (int x = (block_num + 1) * block_size; x < n_examples; x++) {
                total_dist = minkowskiDist(validation_block[i - (block_num * block_size)], seeds_dataset_shuffled[x], minkowskiOrder, n_attributes);
                dist_array[new_index][0] = total_dist;
                dist_array[new_index][1] = seeds_dataset_shuffled[x][n_attributes];
                new_index++;
            }

            qsort(dist_array, n_examples - block_size, 2*sizeof(float), float_compare);

            int count_class[10] = {};
            int class_label = count_classes(block_size, n_examples, dist_array,n_neighbors,10,count_class);


            if (class_label != (int)validation_block[i - (block_num * block_size)][n_attributes]) {
                misclassified += 1;
            }
            if (class_label == (int)validation_block[i - (block_num * block_size)][n_attributes]) {
                classified += 1;
            }
        }
    }

    //float avg_error = misclassified / ((float)n_blocks);
    //return avg_error;

    float avg_acc = classified / (float)n_examples;
    return avg_acc * 100;
}

/*
int main(int argc, char const *argv[])
{
    float err = KNNSpecific("data.txt", 5, 2, 3, 210, 7);
    printf("%f\n", err);
    return 0;
}
*/
