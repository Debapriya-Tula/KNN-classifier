#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//function given to qsort to compare two floating number in an array
// returns 1 if elem1 > elem2, else return -1
int float_compare(const void* elem1, const void* elem2)
{
    return (*(const float*)elem1 - *(const float*)elem2);
}


//function defined to print a 2d array
//parameters:
//  m: first dimension of the array
//  n: second dimension of the array
//  arr: the array to be printed
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

//function to compute the Minkowski distance between two given vectors
//parameters:
//  x: first vector
//  y: second vector
//  order: the order of Minkowski distance
//  num_elements: the number of elements of each vector
//returns:
//  the Minkowski distance of order p between the points x, y
float minkowskiDist(int x[], int y[], int order, int num_elements) {
    float sum = 0;
    for (int i = 0; i < num_elements; i++) {
        float temp = pow(abs(x[i] - y[i]), order);
        sum += temp;
    }

    return pow(sum, 1.0 / order);
}

//function to count the frquency of each class
//parameters:
//  arr: array containing distances and class labels
//  k: the number of nearest neighbors to consider
//  no_of_classes: the number of class labels in the dataset
//  count_class: an array of size no_of_classes kthat will hold the frequency count of each class
//returns:
//  the class label of highest frequency
int count_classes(float arr[][2],int k,int no_of_classes,int count_class[])
{
    //count the frequency of each class
    for(int i = 0; i < k; i++)
        count_class[(int)(arr[i][1])] += 1;

    //loop to iterate over count_class and find the maximum frequency
    int maxPos = 0;
    int max = count_class[0];
    for (int i = 0; i < no_of_classes; i++) {
        if (count_class[i] > max) {
            max = count_class[i];
            maxPos = i;
        }
    }

    //return the class label of maximum frequency
    return maxPos;
}

//function to swap the address locations of two integers
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

//function to run the KNN Classifier on a given dataset for specific k, p
//parameters:
//  filename: the filename of the dataset
//  n_neighbors: the number of nearest neighbors to consider
//  minkowskiOrder: the order of the Minkowski distance to be computed
//  n_blocks: the number of blocks to divide the dataset into
//returns:
//  avg_acc: a floating point number, the accuracy of the classifier with specified hyperparameter
float KNNSpecific(char* filename, int n_neighbors, int minkowskiOrder, int n_blocks, int n_examples, int n_attributes) {
    // Read the file into an array
    srand((unsigned int) time(NULL)); // seed for generating random numbers
    FILE *ptr = fopen(filename, "r"); // open dataset file
    char buff[1000]; // buffer to store values temporarily
    char *token; // token used for delimiting the text file
    int handwritten_dataset[n_examples][n_attributes+1]; // a 2d array to store the dataset
    int handwritten_dataset_shuffled[n_examples][n_attributes+1]; // a 2d array to store the shuffled version of the dataset

    int i=0,j;
    while(fgets(buff,1000,ptr))
    {
        j=0;
        token = strtok(buff, ",\n");
        while (token != NULL && j <= n_attributes + 1)
        {
            handwritten_dataset[i][j] = atof(token);
            token = strtok(NULL, ",\n");
            j++;
        }
        i++;
    } // loop to read the data fromm file



    // Initialise an array for switching indices
    int indices[n_examples]; // an array to store all the indices
    int dest_index = 0;
    for (int i = 0; i < n_examples; i++) {
        indices[i] = i;
    } // populate indices array

    for (int i = 0; i < n_examples; i++) {
        dest_index = rand() % n_examples;
        while(dest_index == i) {
            dest_index = rand() % n_examples;
        } // making sure the generated random number is not equal to the current index

        //swap current index with dest_index
        swap(&indices[i], &indices[dest_index]);
    } // shuffle the indices array

    //shuffle the dataset
    for (int i = 0; i < n_examples; i++) {
    	for (int j = 0; j < n_attributes + 1; j++) {
    		handwritten_dataset_shuffled[i][j] = handwritten_dataset[indices[i]][j];
    	}
    }



    int block_size = n_examples / n_blocks; // initializing size of block
    int count = 0;
    float dist_array[n_examples - block_size][2]; // 2d array to store the distance of one point with all other points along with their class labels
    int classified = 0; // a counter to count the number of correctly classified examples

    int validation_point[n_attributes+1];
    int new_index;
    int class_label;
    float total_dist;
    for (int block_num = 0; block_num < n_blocks; block_num++) {

        for (int i = block_num * block_size; i < (block_num + 1) * block_size; i++) {
            //copying one point into the validation array
            for (int j = 0; j < n_attributes+1; j++) {
                validation_point[j] = handwritten_dataset_shuffled[i][j];
            }

            new_index = 0;
            total_dist = 0;

            for (int x = 0; x < block_num * block_size; x++) {
                // total_dist to compute the mimnkowski distance between two points
                total_dist = minkowskiDist(validation_point, handwritten_dataset_shuffled[x], minkowskiOrder, n_attributes);
                dist_array[new_index][0] = total_dist;
                dist_array[new_index][1] = handwritten_dataset_shuffled[x][n_attributes];
                new_index++;
                //printf("First block %f",handwritten_dataset_shuffled[x][7]);
            }

            for (int x = (block_num + 1) * block_size; x < n_examples; x++) {
                total_dist = minkowskiDist(validation_point, handwritten_dataset_shuffled[x], minkowskiOrder, n_attributes);
                dist_array[new_index][0] = total_dist;
                dist_array[new_index][1] = handwritten_dataset_shuffled[x][n_attributes];
                new_index++;
            }

            qsort(dist_array, n_examples - block_size, 2*sizeof(float), float_compare);
            //print_array(10, 2, dist_array);
            //printf("\n\n\n\n");

            int count_class[10] = {0};
            class_label = count_classes(dist_array,n_neighbors,10,count_class);
            //printf("class = %d,  true class = %d\n", class_label, validation_block[i - (block_num * block_size)][64]);
            //printf("\n\n\n\n\n\n");

            if (class_label == validation_point[n_attributes]) {
                classified += 1; // incrementing once found correctly classified exmaple
            }
        }
        //break;
    }
    printf("Classified correctly: %d\n",classified);
    float avg_acc = classified / ((float)n_examples);
    return avg_acc * 100; // returning the average accuracy of the classifier
}
