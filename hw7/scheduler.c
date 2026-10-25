#include <stdio.h>
#include <string.h>

#define MAX_PROCESSES 100

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;  
    int response_time;
    int is_done;
};

struct GanttBlock {
    int pid;
    int start;
    int end;
};

void compute_times(struct Process *p, int start) {
    p->completion_time = start + p->burst_time;
    p->turnaround_time = p->completion_time - p->arrival_time;
    p->waiting_time    = p->turnaround_time - p->burst_time;
    p->response_time   = start - p->arrival_time;
}

void print_gantt(struct GanttBlock g[], int count) {
    printf("\nGantt Chart:\n");
    printf("|");
    for (int i = 0; i < count; i++) {
        if (g[i].pid == 0)
            printf(" IDLE |");
        else
            printf("  P%-2d |", g[i].pid);
    }
    printf("\n");

    printf("%-7d", g[0].start);
    for (int i = 0; i < count; i++)
        printf("%-7d", g[i].end);
    printf("\n");
}

void print_table(struct Process p[], int n) {
    double total_wt = 0, total_tat = 0, total_rt = 0;

    printf("\n%-5s %-8s %-6s %-8s %-11s %-9s\n",
           "PID", "Arrival", "Burst", "Waiting", "Turnaround", "Response");

    for (int id = 1; id <= n; id++) {
        for (int i = 0; i < n; i++) {
            if (p[i].pid == id) {
                printf("P%-4d %-8d %-6d %-8d %-11d %-9d\n",
                       p[i].pid, p[i].arrival_time, p[i].burst_time,
                       p[i].waiting_time, p[i].turnaround_time, p[i].response_time);
            }
        }
    }

    for (int i = 0; i < n; i++) {
        total_wt  += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
        total_rt  += p[i].response_time;
    }

    printf("\nAverage Waiting Time:    %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Response Time:   %.2f\n", total_rt / n);
}

void fcfs(struct Process p[], int n) {
    struct GanttBlock gantt[2 * MAX_PROCESSES];
    int g = 0;                                

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time ||
                (p[j].arrival_time == p[j + 1].arrival_time && p[j].pid > p[j + 1].pid)) {
                struct Process tmp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = tmp;
            }
        }
    }

    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (p[i].arrival_time > current_time) {
            gantt[g++] = (struct GanttBlock){0, current_time, p[i].arrival_time};
        }
        int start = (current_time > p[i].arrival_time) ? current_time : p[i].arrival_time;

        compute_times(&p[i], start);
        gantt[g++] = (struct GanttBlock){p[i].pid, start, p[i].completion_time};

        current_time = p[i].completion_time;
    }

    printf("\n========== FCFS (First Come First Served) ==========\n");
    print_gantt(gantt, g);
    print_table(p, n);
}


void sjf(struct Process p[], int n) {
    struct GanttBlock gantt[2 * MAX_PROCESSES];
    int g = 0;

    for (int i = 0; i < n; i++)
        p[i].is_done = 0;

    int current_time = 0;
    int finished = 0;

    while (finished < n) {
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (p[i].is_done || p[i].arrival_time > current_time)
                continue;

            if (idx == -1 ||
                p[i].burst_time < p[idx].burst_time ||
                (p[i].burst_time == p[idx].burst_time &&
                 p[i].arrival_time < p[idx].arrival_time)) {
                idx = i;
            }
        }

        if (idx == -1) {
            int next_arrival = -1;
            for (int i = 0; i < n; i++) {
                if (!p[i].is_done &&
                    (next_arrival == -1 || p[i].arrival_time < next_arrival)) {
                    next_arrival = p[i].arrival_time;
                }
            }
            gantt[g++] = (struct GanttBlock){0, current_time, next_arrival};
            current_time = next_arrival;
            continue;
        }

        int start = current_time;
        compute_times(&p[idx], start);
        gantt[g++] = (struct GanttBlock){p[idx].pid, start, p[idx].completion_time};

        p[idx].is_done = 1;
        finished++;
        current_time = p[idx].completion_time;
    }

    printf("\n========== SJF (Shortest Job First, non-preemptive) ==========\n");
    print_gantt(gantt, g);
    print_table(p, n);
}


int main(void) {
    int n;
    struct Process input[MAX_PROCESSES];

    printf("Enter the number of processes: ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_PROCESSES) {
        printf("Invalid number of processes (must be 1 to %d).\n", MAX_PROCESSES);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        input[i].pid = i + 1;
        printf("Enter the arrival time and burst time for process %d: ", i + 1);
        if (scanf("%d %d", &input[i].arrival_time, &input[i].burst_time) != 2 ||
            input[i].arrival_time < 0 || input[i].burst_time <= 0) {
            printf("Invalid input (arrival must be >= 0, burst must be > 0).\n");
            return 1;
        }
    }

    struct Process copy[MAX_PROCESSES];

    memcpy(copy, input, sizeof(struct Process) * n);
    fcfs(copy, n);

    memcpy(copy, input, sizeof(struct Process) * n);
    sjf(copy, n);

    return 0;
}
