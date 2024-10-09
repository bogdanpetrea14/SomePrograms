#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "structs.h"
#include "vma.h"

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);				        \
		}							\
	} while (0)

/*
Valoarea maxima a intului este un numar de 10 cifre
la care se adauga \0, pentru a putea folosi functia
atoi sa se faca conversia.
*/
int convert(FILE *f)
{
	int i = 0;
	fread(&i, sizeof(int), 1, f);
	return i;
}

float convert_float(FILE *f)
{
	float i;
	fread(&i, sizeof(i), 1, f);
	return i;
}

int count_pmu(FILE *f, int count) {
	int pm = 0;//numara cate PMU-uri sunt in lista
	for (int i = 0; i < count; i++) {
		int type = convert(f);
		if (type == 1)
			pm++;
		if (type == 0) {
			fseek(f, 4 * sizeof(float), 1);
			int how = convert(f);
			fseek(f, how * sizeof(int), 1);
		}
		if (type == 1) {
			fseek(f, 5 * sizeof(float), 1);
			int how = convert(f);
			fseek(f, how * sizeof(int), 1); 
		}
	}
	return pm;
}

void read_datas(FILE *f, sensor **sensors, int count)
{
	int i = 0, k = count_pmu(f, count);
	fseek(f, sizeof(int), 0);
	for (int i = 0; i < count; i++) {
		sensors[i] = (sensor *)malloc(sizeof(sensor));
		DIE(!sensors[i], "Cannot alloc memory for this sensor");

		sensors[i]->sensor_data = (int *)malloc(5 * sizeof(float));
		DIE(!sensors[i]->sensor_data, "Cannot alloc memory to save data");
	}
	while (count) {
		int type = convert(f);
		if (type == 0) {
			sensors[k]->sensor_type = TIRE;
			for (int j = 0; j < 4; j++)
				*((float*)sensors[k]->sensor_data + j) = convert_float(f);
			sensors[k]->nr_operations = convert(f);
			sensors[k]->operations_idxs = (int *)malloc(sensors[k]->nr_operations * sizeof(int));
			DIE(!sensors[k]->operations_idxs, "Cant alloc memory for operations");

			for (int j = 0; j < sensors[k]->nr_operations; j++)
				sensors[k]->operations_idxs[j] = convert(f);
			k++;
			count--;
		} else {
			sensors[i]->sensor_type = PMU;
			for (int j = 0; j < 5; j++)
				*((float*)sensors[i]->sensor_data + j) = convert_float(f);
			sensors[i]->nr_operations = convert(f);
			sensors[i]->operations_idxs = (int *)malloc(sensors[i]->nr_operations * sizeof(int));
			DIE(!sensors[i]->operations_idxs, "Cant alloc memory for operations");

			for (int j = 0; j < sensors[i]->nr_operations; j++)
				sensors[i]->operations_idxs[j] = convert(f);
			i++;
			count--;
		}
	}
}

int transform_command(char *command)
{
	char *token = strtok(command, " ");
	if (!strcmp(token, "print"))
		return 1;
	if (!strcmp(token, "analyze"))
		return 2;
	if (!strcmp(token, "clear"))
		return 3;
	if (!strcmp(token, "exit"))
		return 0;
	return 5;
}

void print_sensors(sensor **sensors, int which, int count)
{
	if (which < 0) {
		printf("Index not in range!\n");
		return;
	}
	if (which > count) {
		printf("Index not in range!\n");
		return;
	}
	int j = 0, i = which;
	if (sensors[i]->sensor_type == TIRE) {
		printf("Tire Sensor\n");
		printf("Pressure: %.2f\n", *((float*)sensors[i]->sensor_data + j++));
		printf("Temperature: %.2f\n", *((float*)sensors[i]->sensor_data + j++));
		printf("Wear Level: %d%%\n", *((int*)sensors[i]->sensor_data + j++));
		if (*((float*) sensors[i]->sensor_data + j))
			printf("Performance Score: %d\n", *((int*) sensors[i]->sensor_data + j++));
		else printf("Performance Score: Not Calculated\n");
	}
	if (sensors[i]->sensor_type == PMU) {
		printf("Power Management Unit\n");
		printf("Voltage: %.2f\n", *((float*)sensors[i]->sensor_data + j++));
		printf("Current: %.2f\n", *((float*)sensors[i]->sensor_data + j++));
		printf("Power Consumption: %.2f\n", *((float *)sensors[i]->sensor_data + j++));
		printf("Energy Regen: %d%%\n", *((int*)sensors[i]->sensor_data + j++));
		printf("Energy Storage: %d%%\n", *((int*)sensors[i]->sensor_data + j++));
	}
}

void done(sensor **sensors, int count)
{
	for (int i = 0; i < count; i++) {
		free(sensors[i]->sensor_data);
		free(sensors[i]->operations_idxs);
		free(sensors[i]);
	}
	free(sensors);
}

void analyse(sensor **sensors, int an)
{
	tire_sensor *aux;
	power_management_unit *aux1;
	int j = 0;
	void *operations[8];
	get_operations(operations);
	int how = sensors[an]->nr_operations;
	if (sensors[an]->sensor_type == TIRE) {
		aux = (tire_sensor *)malloc(sizeof(tire_sensor));
		DIE(!aux, "Cannot alloc memory to analyse");

		aux->pressure = *((float*)sensors[an]->sensor_data + j++);
		aux->temperature = *((float*)sensors[an]->sensor_data + j++);
		aux->wear_level = *((int*)sensors[an]->sensor_data + j++);
		aux->performace_score = *((int*)sensors[an]->sensor_data + j);
		for (int i = 0; i < how; i++) {
			int which = sensors[an]->operations_idxs[i];
			((void (*)(void *)) operations[which])(aux);
		}
		j = 0;
		*((float*)sensors[an]->sensor_data + j++) = aux->pressure;
		*((float*)sensors[an]->sensor_data + j++) = aux->temperature;
		*((int*)sensors[an]->sensor_data + j++) = aux->wear_level;
		*((int*)sensors[an]->sensor_data + j++) = aux->performace_score;
		free(aux);
	} else {
		aux1 = (power_management_unit *)malloc(sizeof(power_management_unit));
		DIE(!aux1, "Cannot alloc memory to analyse");

		aux1->voltage = *((float*)sensors[an]->sensor_data + j++);
		aux1->current = *((float*)sensors[an]->sensor_data + j++);
		aux1->power_consumption = *((float*)sensors[an]->sensor_data + j++);
		aux1->energy_regen = *((int*)sensors[an]->sensor_data + j++);
		aux1->energy_storage = *((int*)sensors[an]->sensor_data + j);
		for (int i = 0; i < how; i++) {
			int which = sensors[an]->operations_idxs[i];
			((void (*)(void *)) operations[which])(aux1);
		}
		j = 0;
		*((float*)sensors[an]->sensor_data + j++) = aux1->voltage;
		*((float*)sensors[an]->sensor_data + j++) = aux1->current;
		*((float*)sensors[an]->sensor_data + j++) = aux1->power_consumption;
		*((int*)sensors[an]->sensor_data + j++) = aux1->energy_regen;
		*((int*)sensors[an]->sensor_data + j++) = aux1->energy_storage;
		free(aux1);
	}
}

void remove_sensor(sensor **sensors, int position, int count)
{
	free(sensors[position]->sensor_data);
	free(sensors[position]->operations_idxs);
	free(sensors[position]);
	for (int i = position; i < count - 1; i++) {
		sensors[i] = sensors[i + 1];
	}
}

int clear(sensor **sensors, int count)
{
	for (int i = 0; i < count; i++) {
		int j = 0;
		if (sensors[i]->sensor_type == TIRE) {
			float pres = *((float*)sensors[i]->sensor_data + j++);
			float temp = *((float*)sensors[i]->sensor_data + j++);
			int wl = *((int*)sensors[i]->sensor_data + j++);
			if ((pres < 19 || pres > 28) || 
				(temp < 0 || temp > 120) || (wl < 0 || wl > 100)) {
				remove_sensor(sensors, i, count);
				i--;
				count--;
			}
		} else {
			float v = *((float*)sensors[i]->sensor_data + j++);
			float c = *((float*)sensors[i]->sensor_data + j++);
			float pc = *((float*)sensors[i]->sensor_data + j++);
			int er = *((int*)sensors[i]->sensor_data + j++);
			int es = *((int*)sensors[i]->sensor_data + j++);
			if ((v < 10 || v > 20) || (c < -100 || c > 100) || 
				(pc < 0 || pc > 1000) || (er < 0 || er > 100) || 
				(es < 0 || es > 100)) {
				remove_sensor(sensors, i, count);
				i--;
				count--;
			}
		}
	}
	return count;
}
 
int main(int argc, char const *argv[])
{
	if (argc == 1) {
		printf("No input file");
		return 0;
	}
	char command[BUFSIZ], copy[BUFSIZ], trash[BUFSIZ];
	int count, ok, which, an;
	sensor **sensors;
	FILE *f = fopen(argv[1], "rb");
	DIE(!f, "Cannot open the file");
	count = convert(f);
	sensors = (sensor **)malloc(count * sizeof(sensor*));
	DIE(!sensors, "Cannot alloc memory for the sensors vector:");
	read_datas(f, sensors, count);
	do {
		fgets(command, BUFSIZ, stdin);
		sscanf(command, "%s", copy);
		ok = transform_command(copy);
		switch (ok) {
		case 1:
			sscanf(command, "%s%d", trash, &which);
			print_sensors(sensors, which, count);
		break;
		case 2:
			sscanf(command, "%s%d", trash, &an);
			analyse(sensors, an);
		break;
		case 3:
			count = clear(sensors, count);
		break;
		case 0:
			done(sensors, count);
		break;
		}
	} while (ok);
	fclose(f);
	return 0;
}
