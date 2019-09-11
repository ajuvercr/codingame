#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int at;
    int size;
    void** values;
} Vec;

Vec vec_new() {
    int size = 5;
    void** values = malloc(size * sizeof(void*));
    Vec out = {
        0, size, values,
    };
    return out;
}

void vec_push(Vec* vec, void* value) {
    if (vec->size == vec->at) {
        vec->size *= 2;
        vec->values = realloc(vec->values, sizeof(void*) * vec->size);
    }
    vec->values[vec->at++] = value;
}

void* vec_get(Vec* vec, int index) {
    if (index >= vec->at) {
        return 0;
    }
    return vec->values[index];
}


typedef struct {
    float x;
    float start;
    float end;
} Hole;

Hole* hole_new(float x, float start, float end) {
    Hole* out = malloc(sizeof(Hole));
    out->x = x;
    out->start = start;
    out->end = end;
    return out;
}

typedef struct {
    float x;
    float freq;
} Line;

Line* line_new(float x, float freq) {
    Line* out = malloc(sizeof(Line));
    out->x = x;
    out->freq = freq;
    return out;
}

float max(float x, float  y) {
    if (x > y) {
        return x;
    }
    return y;
}

float min(float x, float  y) {
    if (x < y) {
        return x;
    }
    return y;
}

// expand hole for distance, and get overlap with line, insert new holes in holes
void apply(Vec* holes, Hole* hole, Line* line) {
    hole->start *= line->x / hole->x;
    hole->end *= line->x / hole->x;

    // TODO make fast
    float at = 0.0;
    while(at < hole->end) {
        float start = max(hole->start, at);
        float end = min(hole->end, at + line->freq);
        if (start < end) {
            Hole* h = hole_new(line->x, start, end);

            vec_push(holes, h);
        }
        at += line->freq * 2.0;
    }
}

Vec apply_list(Vec holes, Line* line) {
    Vec out = vec_new();

    for(int i = 0; i < holes.at; i++) {
        Hole* hole = vec_get(&holes, i);
        apply(&out, hole, line);
    }

    return out;
}

int main()
{
    Vec lights = vec_new();

    int s;
    scanf("%d", &s);
    // Everything should be m/s
    float speed = s / 3.6;
    int light_count;
    scanf("%d", &light_count);

    // fastest time to get to the last light
    float first_possible;

    for (int i = 0; i < light_count; i++) {
        int distance;
        int duration;
        scanf("%d%d", &distance, &duration);

        Line* l = line_new(distance, duration);
        vec_push(&lights, l);

        first_possible = distance / speed;
    }

    // Iteratively start with one window of the first light
    int first_index = 0;
    Line* first_line = vec_get(&lights, 0);

    while (1) {
        Vec holes = vec_new();
        Hole b = {
            first_line->x,
            first_index * first_line->freq * 2,
            first_index * first_line->freq * 2 + first_line->freq,
        };
        first_index ++;

        vec_push(&holes, &b);


        // Apply the other lights
        for(int i = 1; i < lights.at; i++) {
            Line* l = (Line*) vec_get(&lights, i);

            holes = apply_list(holes, l);
        }

        // Check if there is a solution that is slower then the max speed
        for (int i = 0; i < holes.at; i++ ) {
            Hole* l = (Hole*) vec_get(&holes, i);

            fprintf(stderr, "%f %f %f\n", first_possible * 1.02, l->end, l->x / l->start * 3.6);
            if (first_possible * 1.02 < l->end) {
                // Some math to get it in the right format
                float answer = l->x / max(l->start, first_possible) * 3.6;
                fprintf(stderr, "%f\n", answer);
                printf("%.0f\n", floor(answer + 0.0001));
                return 0;
            }
        }
    }


    return 0;
}
