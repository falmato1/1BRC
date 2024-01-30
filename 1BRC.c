#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define TABLE_SIZE 1000000

struct row
{
	char city[100];
	float min;
	float max;
	float sum;
	int index;
};

void merge(struct row *rows, int l, int m, int r)
{
	int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    // Create temp arrays
    struct row *L = malloc(n1*sizeof(struct row)); 
	struct row *R = malloc(n2*sizeof(struct row));
 
    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = rows[l + i];
    for (j = 0; j < n2; j++)
        R[j] = rows[m + 1 + j];
 
    // Merge the temp arrays back into arr[l..r
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (strcmp(L[i].city,R[j].city) <= 0) {
            rows[k] = L[i];
            i++;
        }
        else {
            rows[k] = R[j];
            j++;
        }
        k++;
    }
 
    // Copy the remaining elements of L[],
    // if there are any
    while (i < n1) {
        rows[k] = L[i];
        i++;
        k++;
    }
 
    // Copy the remaining elements of R[],
    // if there are any
    while (j < n2) {
        rows[k] = R[j];
        j++;
        k++;
    }
	free(L);
	free(R);
}

void mergeSort(struct row *rows, int l, int r)
{
	if(l < r)
	{
		int m = l + (r - l) / 2;
		mergeSort(rows, l, m);
        mergeSort(rows, m + 1, r);

        merge(rows, l, m, r);
	}
}

unsigned int hash(const char *key)
{
	unsigned int hash = 0;
	for (int i = 0; i < strlen(key) - 1; i++)
	{
		hash += (unsigned)key[i];
		hash = (hash * key[i]) % TABLE_SIZE;
	}
	return hash;
}

void set_val(unsigned int key, struct row *rows, const char *city, float temp)
{	
	if(strlen(rows[key].city) == 0)
	{
		strcpy(rows[key].city, city);
		rows[key].sum = temp;
		rows[key].min = temp;
		rows[key].max = temp;
		rows[key].index = 1;
	}
	else if(strcmp(rows[key].city, city) == 0 && strlen(rows[key].city) != 0)
	{
		rows[key].sum = rows[key].sum + temp;
		if(rows[key].min > temp)
			rows[key].min = temp;
		if(rows[key].max < temp)
		{
			rows[key].max = temp;
		}
		rows[key].index++;
	}
	else
	{
		char num[100];
		sprintf(num, "%d", key);
		//printf("city = %s rows[key] = %s key = %d\n", city, rows[key].city, key);
		unsigned int new_key = hash(num);
		set_val(new_key, rows, city, temp);
	}
	
}

int main(int argc, char *argv[])
{
	clock_t begin1 = clock();

	struct row *rows = malloc(TABLE_SIZE * sizeof(struct row));
	for(int i = 0; i < TABLE_SIZE; i++)
	{
		strcpy(rows[i].city, "\0");
		rows[i].sum = 0.0;
		rows[i].min = 0.0;
		rows[i].max = 0.0;
		rows[i].index = 0;
	}

	FILE *input = fopen("measurements.txt", "r");
	FILE *output = fopen("output", "w");
	char name[1000];
	
	while(fgets(name, 1000, input) != NULL)
	{
		//clock_t begin = clock();
		
		int size = strlen(name);
		char city[size];
		char temp[1000];
		int point = 0;
		int i = 0;
		int j = 0;
		int z = 0;
		
		while(name[i] != '\0')
		{
			if(name[i] == ';')
			{
				j = i + 1;
				point = i;
			}
			if(j > i)
			{
				temp[z] = name[j];
				j++;
				z++;
			}
			if(j == 0)
			{
				city[i] = name[i];
				
			}
			i++;
		}	
		
		city[point] = '\0';	
		temp[z] = '\0';
		float temp_num = atof(temp);
		unsigned int key = hash(city);
		set_val(key, rows, city, temp_num);

	}
	
	clock_t end = clock();
	double time_spent1 = (double)((end - begin1) / CLOCKS_PER_SEC);
	
	mergeSort(rows, 0, TABLE_SIZE - 1);
	
		
	for(int i = 0; i < TABLE_SIZE; i++)
	{
		if(strcmp(rows[i].city, "\0") != 0)
			fprintf(output, "<%s, min = %.1f, mean = %.1f, max = %.1f>\n",rows[i].city, rows[i].min, (float)(rows[i].sum/rows[i].index), rows[i].max);
	}
	
	clock_t end1 = clock();

	double time_spent = (double)((end1 - begin1) / CLOCKS_PER_SEC);
	printf("%f\n", time_spent1);

	printf("%f\n", time_spent);
	

	free(rows);
	//free(rows1);
	fclose(input);
	fclose(output);
	return 0;

}
