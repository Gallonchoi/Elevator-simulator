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
 * 用户状态
 */
typedef enum {
    Waiting,  // 等待电梯中
    Accessing,  // 进或出电梯中
    Ongoing  // 在电梯内, 到达目的楼层途中
} custState;

/**
 * 用户
 */
typedef struct {
    int id;
    int in_floor;  // 源楼层
    int out_floor;  // 目标楼层
    int patient_time;  // 容忍时间
    int waiting_time;  // 等待时间
    int access_time;  // 进或出电梯耗费时间
    custState state;
} * customerStru, custStru;

/**
 * 电梯
 */
typedef struct {
    int id;
    int base_position;  // 初始位置
    int current_position;  // 当前位置
    int state_time;  // 当前状态时间
    int customer;  // 当前载有用户数量
    int move_time;  // 上升或下降一层耗费的单位时间
    int gate_op_time;  // 开门或关门耗费的时间
    elevState state;  // 当前状态
    customerStru customers[50];  // 载有用户
} * elevatorStru, elevStru;

/**
 * 模拟器
 */
typedef struct {
    int customer;  // 用户数量
    int elevator;  // 电梯数量
    int base_floor;  // 默认本垒层
    int min_floor;  // 默认最底层
    int max_floor;  // 默认最顶层
    int cust_patient;  // 用户默认容忍时间
    int cust_access_time;  // 用户默认进或出电梯耗费时间
    int current_time;  // 当前单位时间
    int calling_number;  // 呼叫信号数量
    int calling_queue[120];  // 电梯呼叫信号队列, 保存呼叫楼层
    customerStru customer_queue[100];  // 用户队列
    elevatorStru elevator_queue[10];  // 电梯队列
} * simulatorStru, simStru;

/**
 * 获取新电梯 id
 *
 * @return int
 */
int get_elevator_id() {
    return elevator_id ++;
}

/**
 * 获取新用户 id
 *
 * @return int
 */
int get_customer_id() {
    return customer_id ++;
}

/**
 * 获取电梯状态解释字符串
 *
 * @return char *
 */
char * get_elev_state(elevState state) {
    char * description[30];
    *description = (char *) malloc (sizeof(char));
    switch(state) {
    case Uping:
        *description = "上升中";
        break;
    case Downing:
        *description = "下降中";
        break;
    case Idle:
        *description = "静止中";
        break;
    case Opening:
        *description = "开门中";
        break;
    case Closing:
        *description = "关门中";
        break;
    }
    return *description;
}

/**
 * 获取用户状态解释字符串
 *
 * @return char *
 */
char * get_cust_state(custState state) {
    char * description[30];
    *description = (char *) malloc (sizeof(char));
    switch(state) {
    case Waiting:
        *description = "等待电梯中";
        break;
    case Accessing:
        *description = "进出电梯中";
        break;
    case Ongoing:
        *description = "在电梯内";
        break;
    }
    return *description;
}

/**
 * 搜索呼叫信号是否存在
 *
 * @param simulator 模拟器
 * @param floor 呼叫楼层
 * @return bool
 */
bool search_calling(simulatorStru * simulator, int floor) {
    int idx;
    for(idx = 0; idx < (*simulator)->calling_number; idx ++) {
        if((*simulator)->calling_queue[idx] == floor) {
            return true;
        }
    }
    return false;
}

/**
 * 设置新的呼叫信号
 *
 * @param simulator 模拟器
 * @param floor 呼叫楼层
 * @return void
 */
void set_new_calling(simulatorStru * simulator, int floor) {
    if(search_calling(simulator, floor) == false) {
        // 直接插入排序
        int idx;
        for(idx = (*simulator)->calling_number; idx > 0; idx ++) {
            if((*simulator)->calling_queue[idx - 1] < floor) {
                // 插入新元素
                (*simulator)->calling_queue[idx] = floor;
                break;
            } else {
                // 后移元素
                (*simulator)->calling_queue[idx] = (*simulator)->calling_queue[idx - 1];
                if(idx == 1) {
                    // 插入到记录头部
                    (*simulator)->calling_queue[0] = floor;
                }
            }
        }
    }
}

#endif /* CORE_H */
