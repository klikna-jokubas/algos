#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define KEY_LENGTH 30
#define SYNONYMS_LENGTH 300
#define TABLE_SIZE 38197

typedef struct entry_ {
    int occupied;
    char key[KEY_LENGTH];
    char synonyms[SYNONYMS_LENGTH];
} entry;

typedef entry *closed_table;
typedef int pos;

unsigned int hashA(char *key, int tableSize) {
    int i, n = MIN(8, strlen(key));
    unsigned int value = key[0];
    for (i = 1; i < n; i++) value += key[i];
    return value % tableSize;
}

unsigned int hashB(char *key, int tableSize) {
    int i, n = MIN(8, strlen(key));
    unsigned int value = key[0];
    for (i = 1; i < n; i++) value = (value << 5) + key[i];
    return value % tableSize;
}


unsigned int linear_probing(int initial_pos, int try_number) {
    return (initial_pos + try_number) % TABLE_SIZE;
}

unsigned int quadratic_probing(int initial_pos, int try_number) {
    return (initial_pos + try_number * try_number) % TABLE_SIZE;
}

unsigned int double_hashing(int initial_pos, int try_number) {
    int secondary_hash = 10007 - (initial_pos % 10007);
    return (initial_pos + try_number * secondary_hash) % TABLE_SIZE;
}

void init_closed(closed_table *dictionary, int size) {
    *dictionary = (closed_table)malloc(size * sizeof(entry));
    if (*dictionary == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        (*dictionary)[i].occupied = 0;
        (*dictionary)[i].key[0] = '\0';
        (*dictionary)[i].synonyms[0] = '\0';
    }
}

int insert_closed(char *key, char *synonyms, closed_table *dictionary, int size,
                 unsigned int (*hash)(char *, int),
                 unsigned int (*collision_resolution)(int initial_pos, int try_number)) {
    int initial_pos = hash(key, size);
    int collisions = 0;
    int pos = initial_pos;

    for (int i = 0; i < size; i++) {
        if (!(*dictionary)[pos].occupied) {
            (*dictionary)[pos].occupied = 1;
            strncpy((*dictionary)[pos].key, key, KEY_LENGTH - 1);
            (*dictionary)[pos].key[KEY_LENGTH - 1] = '\0';
            strncpy((*dictionary)[pos].synonyms, synonyms, SYNONYMS_LENGTH - 1);
            (*dictionary)[pos].synonyms[SYNONYMS_LENGTH - 1] = '\0';
            return collisions;
        }
        collisions++;
        pos = collision_resolution(initial_pos, i + 1);
    }
    return -1;
}

pos find_closed(char *key, closed_table dictionary, int size, int *collisions,
                unsigned int (*hash)(char *, int),
                unsigned int (*collision_resolution)(int initial_pos, int try_number)) {
    int initial_pos = hash(key, size);
    int pos = initial_pos;
    *collisions = 0;

    for (int i = 0; i < size; i++) {
        if (dictionary[pos].occupied && strcmp(dictionary[pos].key, key) == 0) {
            return pos;
        }
        if (!dictionary[pos].occupied) break;
        (*collisions)++;
        pos = collision_resolution(initial_pos, i + 1);
    }
    return -1;
}

void show_closed(closed_table dictionary, int size) {
    printf("{\n");
    for (int i = 0; i < size; i++) {
        if (dictionary[i].occupied) {
            printf("%d- (%s )\n", i, dictionary[i].key);
        } else {
            printf("%d-\n", i);
        }
    }
    printf("}\n");
}

int read_synonyms(entry data[]) {
    char c;
    int i, j;
    FILE *myfile;

    if ((myfile = fopen("sinonimos.txt", "r")) == NULL) {
        printf("Error opening 'sinonimos.txt'\n");
        return EXIT_FAILURE;
    }

    for (i = 0; fscanf(myfile, "%s", data[i].key) != EOF; i++) {
        if ((c = fgetc(myfile)) != '\t') {
            printf("Error reading a tab\n");
            fclose(myfile);
            return EXIT_FAILURE;
        }

        for (j = 0; (c = fgetc(myfile)) != '\n'; j++) {
            if (j < SYNONYMS_LENGTH - 1)
                data[i].synonyms[j] = c;
        }
        data[i].synonyms[MIN(j, SYNONYMS_LENGTH - 1)] = '\0';
    }

    if (fclose(myfile) != 0) {
        printf("Error closing the file\n");
        return EXIT_FAILURE;
    }

    return i;
}

double measure_search_time(closed_table dictionary, entry data[], int n, int size,
                           unsigned int (*hash)(char *, int),
                           unsigned int (*collision_resolution)(int initial_pos, int try_number)) {
    clock_t start, end;
    int collisions;
    int K = 100;
    int *indices = (int *)malloc(n * sizeof(int)); 
    if (indices == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        indices[i] = rand() % n;
    }
    
    start = clock();
    for (int k = 0; k < K; k++) { 
        for (int i = 0; i < n; i++) {
            find_closed(data[indices[i]].key, dictionary, size, &collisions, hash, collision_resolution);
        }
    }
    end = clock();

    free(indices);
    
    return ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0 / K; 
}



void analyze_complexity(closed_table dictionary, entry data[], int size,
                       unsigned int (*hash)(char *, int),
                       unsigned int (*collision_resolution)(int initial_pos, int try_number)) {
    int sizes[] = {125, 250, 500, 1000, 2000, 4000, 8000, 16000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("\nSize\tTime(ms)\tt(n)/n^0.8\tt(n)/n\tt(n)/(n*log(n))\n");
    printf("--------------------------------------------------------\n");

    for (int i = 0; i < num_sizes; i++) {
        int n = sizes[i];
        double time = measure_search_time(dictionary, data, n, size, hash, collision_resolution);
        
        double ratio_pow = time / pow(n, 0.8);
        double ratio_linear = time / n;
        double ratio_nlogn = time / (n * log(n));

        printf("%d\t%.3f\t%.6f\t%.6f\t%.6f\n", 
       n, time, ratio_pow, ratio_linear, ratio_nlogn);

    }
}

int main() {
    closed_table dictionary;
    init_closed(&dictionary, TABLE_SIZE);

    entry data[19062];
    int num_entries = read_synonyms(data);
    if (num_entries <= 0) {
        printf("Error reading synonyms file\n");
        free(dictionary);
        return EXIT_FAILURE;
    }
    
    unsigned int (*hash_functions[])(char*, int) = {hashA, hashB};
    unsigned int (*collision_methods[])(int, int) = {
        linear_probing, quadratic_probing, double_hashing
    };
    char *hash_names[] = {"hashA", "hashB"};
    char *method_names[] = {"Linear Probing", "Quadratic Probing", "Double Hashing"};

    for (int h = 0; h < 2; h++) {
        for (int m = 0; m < 3; m++) {
            printf("\n\nTesting %s with %s:\n", hash_names[h], method_names[m]);
            
            init_closed(&dictionary, TABLE_SIZE);
            
            int total_collisions = 0;
            for (int i = 0; i < num_entries; i++) {
                int collisions = insert_closed(data[i].key, data[i].synonyms,
                                               &dictionary, TABLE_SIZE,
                                               hash_functions[h], collision_methods[m]);
                if (collisions >= 0) total_collisions += collisions;
            }

            printf("Total insertion collisions: %d\n", total_collisions);

            // Analyze search complexity
            analyze_complexity(dictionary, data, TABLE_SIZE,
                               hash_functions[h], collision_methods[m]);
        }
    }
    
    free(dictionary);
    return 0;
}

