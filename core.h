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
    Opened,  // 已开门, 等待用户进入
    Closing  // 关门中
} elevState;

/**
 * 电梯信号
 */
typedef enum {
    Up,
    Down
} elevSignal;

/**
 * 用户状态
 */
typedef enum {
    Waiting,  // 等待电梯中
    Accessing,  // 进或出电梯中
    Ongoing,  // 在电梯内, 到达目的楼层途中
    Leaving  // 用户离开(等待时间达到容忍时间, 或到达目标楼层)
} custState;

/**
 * 用户
 */
typedef struct {
    int id;
    int in_floor;  // 源楼层
    int out_floor;  // 目标楼层
    int patient_time;  // 容忍时间
    int state_time;  // 当前状态时间
    int access_time;  // 进或出电梯耗费时间
    int elevator_id;  // 所在电梯 id
    custState state;
} * customerStru, custStru;

/**
 * 电梯
 */
typedef struct {
    int id;
    int base_floor;  // 初始位置, 非本垒层
    int current_floor;  // 当前位置
    int state_time;  // 当前状态时间
    int customer;  // 当前载有用户数量
    int move_time;  // 上升或下降一层耗费的单位时间
    int gate_op_time;  // 开门或关门耗费的时间
    int destination;  // 目标楼层
    elevState state;  // 当前状态

    int in_floor_front;  // 等待进入用户队列头索引
    int in_floor_rear;  // 等待进入用户队列尾索引
    customerStru in_floor_queue[50];  // 等待进入用户队列

    int out_floor_front;  // 等待出去用户队列头索引
    int out_floor_rear;  // 等待出去用户队列尾索引
    customerStru out_floor_queue[50];  // 等待出去用户队列

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
    int max_idle_time;  // 电梯静止最大时间, 达到则返回本垒层
    int current_time;  // 当前单位时间

    int calling_up_number;  // 呼叫上升信号数量
    int calling_down_number;  // 呼叫下降信号数量
    int calling_up_queue[120];  // 电梯上升呼叫信号队列, 保存呼叫楼层
    int calling_down_queue[120];  // 电梯下降呼叫信号队列, 保存呼叫楼层

    customerStru customer_queue[100];  // 用户队列
    elevatorStru elevator_queue[10];  // 电梯队列
} * simulatorStru, simStru;

void go_to(elevatorStru * elevator, int floor);
void delete_up_calling(simulatorStru * simulator, int floor);
void delete_down_calling(simulatorStru * simulator, int floor);

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
    case Opened:
        *description = "完成开门动作, 等待用户进入";
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
    case Leaving:
        *description = "离开";
        break;
    }
    return *description;
}

/**
 * 添加某楼层等待上楼用户到电梯队列
 *
 * @param simulator 模拟器
 * @param elevator 电梯
 * @param floor 目标楼层
 * @return void
 */
void add_up_customer(simulatorStru * simulator, elevatorStru * elevator, int floor) {
    int idx;
    for(idx = 0; idx < (*simulator)->customer; idx ++) {
        customerStru cust;
        cust = (*simulator)->customer_queue[idx];
        if(cust->in_floor == floor && cust->state == Waiting && cust->out_floor > floor) {
            cust->state = Accessing;
            (*elevator)->in_floor_queue[(*elevator)->in_floor_rear++] = cust;
        }
    }
}

/**
 * 添加某楼层等待下楼用户到电梯队列
 *
 * @param simulator 模拟器
 * @param elevator 电梯
 * @param floor 目标楼层
 * @return void
 */
void add_down_customer(simulatorStru * simulator, elevatorStru * elevator, int floor) {
    int idx;
    for(idx = 0; idx < (*simulator)->customer; idx ++) {
        customerStru cust;
        cust = (*simulator)->customer_queue[idx];
        if(cust->in_floor == floor && cust->state == Waiting && cust->out_floor < floor) {
            cust->state = Accessing;
            (*elevator)->in_floor_queue[(*elevator)->in_floor_rear++] = cust;
        }
    }
}

/**
 * 设置上升信号
 *
 * @param simulator 模拟器
 * @param floor 目标楼层
 * @return void
 */
void set_new_up_calling(simulatorStru * simulator, int floor) {
    if((*simulator)->calling_up_number == 0) {
        (*simulator)->calling_up_queue[(*simulator)->calling_up_number ++] = floor;
        return;
    }
    // 直接插入排序
    int idx;
    for(idx = (*simulator)->calling_up_number; idx > 0; idx ++) {
        if((*simulator)->calling_up_queue[idx - 1] == floor) {
            // 重复信号
            return;
        }
        if((*simulator)->calling_up_queue[idx - 1] < floor) {
            // 插入新元素
            (*simulator)->calling_up_queue[idx] = floor;
            break;
        } else {
            // 后移元素
            (*simulator)->calling_up_queue[idx] = (*simulator)->calling_up_queue[idx - 1];
            if(idx == 1) {
                // 插入到记录头部
                (*simulator)->calling_up_queue[0] = floor;
            }
        }
    }
    (*simulator)->calling_up_number ++;
}

/**
 * 设置下降信号
 *
 * @param simulator 模拟器
 * @param floor 目标楼层
 * @return void
 */
void set_new_down_calling(simulatorStru * simulator, int floor) {
    if((*simulator)->calling_down_number == 0) {
        (*simulator)->calling_down_queue[(*simulator)->calling_down_number ++] = floor;
        return;
    }
    // 直接插入排序
    int idx;
    for(idx = (*simulator)->calling_down_number; idx > 0; idx ++) {
        if((*simulator)->calling_down_queue[idx - 1] == floor) {
            // 重复信号
            return;
        }
        if((*simulator)->calling_down_queue[idx - 1] < floor) {
            // 插入新元素
            (*simulator)->calling_down_queue[idx] = floor;
            break;
        } else {
            // 后移元素
            (*simulator)->calling_down_queue[idx] = (*simulator)->calling_down_queue[idx - 1];
            if(idx == 1) {
                // 插入到记录头部
                (*simulator)->calling_down_queue[0] = floor;
            }
        }
    }
    (*simulator)->calling_down_number ++;
}

/**
 * 设置空闲电梯信号
 *
 * @param simulator 模拟器
 * @param elevator 电梯
 * @return void
 */
void get_calling(simulatorStru * simulator, elevatorStru * elevator) {
    int idx;
    // 分配上升信号
    for(idx = 0; idx < (*simulator)->calling_up_number; idx ++) {
        if((*simulator)->calling_up_queue[idx] == (*elevator)->current_floor) {
            delete_up_calling(simulator, (*elevator)->current_floor);
            (*elevator)->state = Opening;
            (*elevator)->state_time = 0;
            return;
        } else if((*simulator)->calling_up_queue[idx] > (*elevator)->current_floor) {
            go_to(elevator, (*simulator)->calling_up_queue[idx]);
            return;
        } else if(idx + 1 == (*simulator)->calling_up_number) {
            go_to(elevator, (*simulator)->calling_up_queue[idx]);
            return;
        }
    }
    // 分配下降信号
    for(idx = 0; idx < (*simulator)->calling_down_number; idx ++) {
        if((*simulator)->calling_down_queue[idx] == (*elevator)->current_floor) {
            delete_down_calling(simulator, (*elevator)->current_floor);
            (*elevator)->state = Opening;
            (*elevator)->state_time = 0;
            return;
        } else if((*simulator)->calling_down_queue[idx] < (*elevator)->current_floor) {
            go_to(elevator, (*simulator)->calling_down_queue[idx]);
            return;
        } else if(idx + 1 == (*simulator)->calling_down_number) {
            go_to(elevator, (*simulator)->calling_down_queue[idx]);
            return;
        }
    }
}

/**
 * 删除某层上升呼叫信号
 *
 * @param elevator 电梯
 * @param floor 目标楼层
 * @return void
 */
void delete_up_calling(simulatorStru * simulator, int floor) {
    int idx, anchor = -1;
    for(idx = 0; idx < (*simulator)->calling_up_number; idx ++) {
        if((*simulator)->calling_up_queue[idx] == floor) {
            anchor = idx;
        }
    }
    for(idx = anchor; idx < (*simulator)->calling_up_number; idx ++) {
        (*simulator)->calling_up_queue[idx] = (*simulator)->calling_up_queue[idx + 1];
    }
    (*simulator)->calling_up_number --;
}

/**
 * 删除某层下降呼叫信号
 *
 * @param elevator 电梯
 * @param floor 目标楼层
 * @return void
 */
void delete_down_calling(simulatorStru * simulator, int floor) {
    int idx, anchor = -1;
    for(idx = 0; idx < (*simulator)->calling_down_number; idx ++) {
        if((*simulator)->calling_down_queue[idx] == floor) {
            anchor = idx;
        }
    }
    for(idx = anchor; idx < (*simulator)->calling_down_number; idx ++) {
        (*simulator)->calling_down_queue[idx] = (*simulator)->calling_down_queue[idx + 1];
    }
    (*simulator)->calling_down_number --;
}

/**
 * 设置电梯目标楼层
 *
 * @param elevator 电梯
 * @param floor 目标楼层
 * @return void
 */
void go_to(elevatorStru * elevator, int floor) {
    if((*elevator)->current_floor == floor) {
        (*elevator)->state = Opening;
        (*elevator)->destination = 0;  // 目标楼层置 0
    } else if((*elevator)->current_floor < floor) {
        (*elevator)->state = Uping;
        (*elevator)->destination = floor;
    } else {
        (*elevator)->state = Downing;
        (*elevator)->destination = floor;
    }
    (*elevator)->state_time = 0;  // 重置当前状态时间
}

/**
 * 从电梯中删除用户
 *
 * @param elevator 电梯
 * @param cust_id 用户 ID
 * @return void
 */
void delete_cust_from_elev(elevatorStru * elevator, int cust_id) {
    int idx, anchor = -1;
    for(idx = 0; idx < (*elevator)->customer; idx ++) {
        customerStru cust;
        cust = (*elevator)->customers[idx];
        if(cust->id == cust_id) {
            anchor = idx;
            break;
        }
    }
    for(idx = anchor; idx < (*elevator)->customer; idx ++) {
        (*elevator)->customers[idx] = (*elevator)->customers[idx + 1];
    }
    (*elevator)->customer --;
}

/**
 * 确定出电梯用户队列
 *
 * @param elevator 电梯
 * @return void
 */
void out_elevator_queue(elevatorStru * elevator) {
    (*elevator)->out_floor_front = (*elevator)->out_floor_rear = 0;
    if((*elevator)->state == Opened) {
        int idx;
        for(idx = 0; idx < (*elevator)->customer; idx ++) {
            customerStru cust;
            cust = (*elevator)->customers[idx];
            if(cust->out_floor == (*elevator)->current_floor) {
                (*elevator)->out_floor_queue[(*elevator)->out_floor_rear++] = cust;  // 添加待出电梯用户到队列
            }
        }
    }
}

/**
 * 用户出电梯, 所有用户出电梯返回 true, 否则返回 false
 *
 * @param elevator 电梯
 * @return bool
 */
bool out_elevator(elevatorStru * elevator) {
    if((*elevator)->out_floor_rear == 0) {
        out_elevator_queue(elevator);
    }
    if((*elevator)->out_floor_front < (*elevator)->out_floor_rear) {
        customerStru cust;
        cust = (*elevator)->out_floor_queue[(*elevator)->out_floor_front];
        cust->state_time ++;
        if(cust->state_time >= cust->access_time) {
            // 用户完成出电梯动作
            cust->state = Leaving;
            cust->state_time = 0;
            delete_cust_from_elev(elevator, cust->id);
            (*elevator)->out_floor_front ++;
            if((*elevator)->in_floor_front == (*elevator)->in_floor_rear) {
                return true;
            }
        }
        return false;
    } else {
        return true;
    }
}

/**
 * 确定进电梯用户队列
 *
 * @param simulator 模拟器
 * @param elevator 电梯
 * @return void
 */
void in_elevator_queue(simulatorStru * simulator, elevatorStru * elevator) {
    int idx;
    for(idx = 0; idx < (*simulator)->customer; idx ++) {
        customerStru cust;
        cust = (*simulator)->customer_queue[idx];
        if(cust->state == Waiting && cust->in_floor == (*elevator)->current_floor) {
            (*elevator)->in_floor_queue[(*elevator)->in_floor_rear ++] = cust;
            cust->state = Accessing;
            cust->state_time = 0;
        }
    }
}

/**
 * 用户进入电梯, 所有用户进入电梯返回 true, 否则返回 false
 *
 * @param elevator 电梯
 * @return bool
 */
bool in_elevator(elevatorStru * elevator) {
    if((*elevator)->in_floor_front < (*elevator)->in_floor_rear) {
        customerStru cust;
        cust = (*elevator)->in_floor_queue[(*elevator)->in_floor_front];
        cust->state_time ++;
        if(cust->state_time >= cust->access_time) {
            // 用户完成进入电梯动作
            cust->state = Ongoing;
            cust->state_time = 0;
            cust->elevator_id = (*elevator)->id;
            (*elevator)->in_floor_front ++;
            (*elevator)->customers[(*elevator)->customer ++] = cust;
            if((*elevator)->in_floor_front == (*elevator)->in_floor_rear) {
                return true;
            }
        }
        return false;
    } else {
        return true;
    }
}

/**
 * 设置电梯目的地
 *
 * @param elevator 电梯
 * @return void
 */
void set_destination(elevatorStru * elevator) {
    int idx, dest;
    dest = 0;
    for(idx = 0; idx < (*elevator)->customer; idx ++) {
        customerStru cust;
        cust = (*elevator)->customers[idx];
        if(dest == 0) {
            dest = cust->out_floor;
        } else if(cust->out_floor > dest && cust->out_floor > (*elevator)->current_floor) {
            dest = cust->out_floor;
        } else if(cust->out_floor < dest && cust->out_floor < (*elevator)->current_floor) {
            dest = cust->out_floor;
        }
    }
    if(dest != 0) {
        go_to(elevator, dest);
    } else {
        (*elevator)->state = Idle;
        (*elevator)->state_time = 0;
        (*elevator)->destination = 0;
    }
}

/**
 * 查看某层是否有上升信号
 *
 * @param simulator 模拟器
 * @param floor 目标楼层
 * @return bool
 */
bool search_up_calling(simulatorStru * simulator, int floor) {
    int idx;
    for(idx = 0; idx < (*simulator)->calling_up_number; idx ++) {
        if((*simulator)->calling_up_queue[idx] == floor) {
            return true;
        } else if((*simulator)->calling_up_queue[idx] > floor) {
            break;
        }
    }
    return false;
}

/**
 * 查看某层是否有下降信号
 *
 * @param simulator 模拟器
 * @param floor 目标楼层
 * @return bool
 */
bool search_down_calling(simulatorStru * simulator, int floor) {
    int idx;
    for(idx = 0; idx < (*simulator)->calling_down_number; idx ++) {
        if((*simulator)->calling_down_queue[idx] == floor) {
            return true;
        } else if((*simulator)->calling_down_queue[idx] > floor) {
            break;
        }
    }
    return false;
}

#endif /* CORE_H */
