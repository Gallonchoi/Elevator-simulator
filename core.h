#ifndef CORE_H
#define CORE_H

#include <stdlib.h>

typedef enum {true, false} bool;

/**
 * 用户, 电梯 ID
 */
int customer_id = 1;
int elevator_id = 1;

/**
 * 电梯状态
 */
typedef enum {
    Uping,  // 上升中
    Downing,  // 下降中
    Idle,  // 静止
    Opening,  // 开门中
    Closing  // 关门中
} elevState;

/**
 * 用户
 */
typedef struct {
    int id;
    int source;  // 源楼层
    int destination;  // 目标楼层
    int patient_time;  // 容忍时间
    int waiting_time;  // 等待时间
    int access_time;  // 进或出电梯耗费时间
} * customerStru, custStru;

/**
 * 电梯
 */
typedef struct {
    int id;
    int base_position;  // 初始位置
    int current_position;  // 当前位置
    int state_time;  // 当前状态时间
    int move_time;  // 上升或下降一层耗费的单位时间
    int gate_op_time;  // 开门或关门耗费的时间
    elevState state;  // 当前状态
    customerStru * customers[50];  // 载有用户
} * elevatorStru, elevStru;

/**
 * 模拟器
 */
typedef struct {
    int client;  // 用户数量
    int elevator;  // 电梯数量
    double unit_of_time;  // 单位时间
    customerStru * client_queue[100];  // 用户队列
    elevatorStru * elevator_queue[10];  // 电梯队列
} * simulatorStru, simStru;

int get_elevator_id() {
    return elevator_id ++;
}

int get_customer_id() {
    return customer_id ++;
}

/**
 * 初始化模拟器
 *
 * @param simulator 模拟器
 * @return void
 */
void init(simulatorStru * simulator) {
    *simulator = (simulatorStru) malloc (sizeof(simStru));
    (*simulator)->client = 0;
    (*simulator)->elevator = 0;
}

/**
 * 添加用户
 *
 * @param simulator 模拟器
 * @param source 用户源楼层
 * @param dest 用户目标楼层
 * @param patient 用户容忍时间
 * @param access 用户进或出电梯耗费的时间
 * @return void
 */
void add_customer(simulatorStru * simulator, int source, int dest, int patient, int access) {
    customerStru customer;
    customer = (customerStru) malloc (sizeof(custStru));
    customer->id = get_customer_id();
    customer->source = source;
    customer->destination = dest;
    customer->patient_time = patient;
    customer->waiting_time = 0;
    customer->access_time = access;
    (*simulator)->client_queue[(*simulator)->client ++] = &customer;
}

/**
 * 添加电梯
 *
 * @param simulator 模拟器
 * @param base 电梯初始位置
 * @return void
 */
void add_elevator(simulatorStru * simulator, int base) {
    elevatorStru elevator;
    elevator = (elevatorStru) malloc (sizeof(elevStru));
    elevator->id = get_elevator_id();
    elevator->base_position = base;
    elevator->current_position = base;
    elevator->state = Idle;
    elevator->state_time = 0;
    (*simulator)->elevator_queue[(*simulator)->elevator ++] = &elevator;
}

#endif /* CORE_H */
