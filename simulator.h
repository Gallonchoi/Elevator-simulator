#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdio.h>
#include "core.h"

void read_command();
void print_cur_state(simulatorStru * simulator);
void print_help();
void print_env();
void next_unit(simulatorStru * simulator, int time);
void init_simulator(simulatorStru * simulator, int elevator, int base_floor, int min_floor, int max_floor);
void add_customer(simulatorStru * simulator, int in_floor, int out_floor, int patient, int access);
void add_elevator(simulatorStru * simulator, int base);
void update_elevator(simulatorStru * simulator);
void update_customer(simulatorStru * simulator);

/**
 * 读取并解析命令
 *
 * @return void
 */
void read_command() {
    simulatorStru simulator;  // 全局模拟器

    init_simulator(&simulator, 3, 1, -1, 10);

    int command_code = 1;  // 命令码
    bool is_exit = false;  // 退出标志

    while(is_exit == false) {
        switch(command_code) {
        case 1:
            // 输出帮助
            print_help();
            break;
        case 2:
            // 输出当前状态
            print_cur_state(&simulator);
            printf("请按任意键返回\n");
            getchar();
            getchar();
            print_help();
            break;
        case 3:
            // 模拟到下一个单位时间
            next_unit(&simulator, 1);
            print_help();
            break;
        case 4:
            // 模拟到下N个单位时间
            {
                int n;
                scanf("%d", &n);
                next_unit(&simulator, n);
                print_help();
            }
            break;
        case 5:
            // 添加用户
            {
                int in_floor, out_floor, patient, access;
                scanf("%d", &in_floor);  // 源楼层
                scanf("%d", &out_floor);  // 目标楼层
                scanf("%d", &patient);  // 目标楼层
                scanf("%d", &access);  // 目标楼层
                add_customer(&simulator, in_floor, out_floor, patient, access);
            }
            break;
        case 6:
            // 设置环境变量
            print_env(&simulator);
            int code, temp;
            scanf("%d", &code);  // 读入命令码
            switch(code) {
            case 0:
                // 设置电梯数量
                printf("电梯数量(1 ~ 9) = ");
                scanf("%d", &temp);
                if(temp < 1 || temp > 9) {
                    printf("=========================\n");
                    printf("参数错误, 电梯数量少于1\n");
                    printf("=========================\n");
                } else {
                    simulator->elevator = temp;
                }
                break;
            case 1:
                // 设置最底楼层
                printf("最底楼层(-9 ~ -1, 1) = ");
                scanf("%d", &temp);
                if(temp == 0 || temp > 1 || temp < -9) {
                    printf("=========================\n");
                    printf("参数错误, 最底楼层不能大于1或等于0\n");
                    printf("=========================\n");
                } else {
                    simulator->min_floor = temp;
                }
                break;
            case 2:
                // 设置最顶楼层
                printf("最顶楼层(2 ~ 99) = ");
                scanf("%d", &temp);
                if(temp < 2 || temp > 99) {
                    printf("=========================\n");
                    printf("参数错误, 最顶楼层不能小于或等于1\n");
                    printf("=========================\n");
                } else {
                    simulator->max_floor = temp;
                }
                break;
            case 3:
                // 设置本垒楼层
                printf("本垒楼层(%d ~ %d) = ", simulator->min_floor, simulator->max_floor);
                scanf("%d", &temp);
                if(temp < simulator->min_floor || temp > simulator->max_floor || temp == 0) {
                    printf("=========================\n");
                    printf("参数错误, 本垒楼层不能小于最底楼层或大于最顶楼层或等于0\n");
                    printf("=========================\n");
                } else {
                    simulator->base_floor = temp;
                }
                break;
            default:
                printf("=========================\n");
                printf("未知命令\n");
                printf("=========================\n");
                break;
            }
            print_env(&simulator);
            printf("请按任意键返回\n");
            getchar();
            getchar();
            print_help();
            break;
        case 7:
            // 重置
            break;
        case 8:
            // 退出
            is_exit = true;
            printf("退出中...\n");
            break;
        default:
            printf("=========================\n");
            printf("未知命令\n");
            printf("=========================\n");
            print_help();
        }
        if(is_exit == false)
            scanf("%d", &command_code);  // 读入命令
    }
}

/**
 * 输出帮助
 *
 * @return void
 */
void print_help() {
    printf(" Elevator simulator\n");
    printf(" Commands:\n");
    printf(" 1         : 输出帮助\n");
    printf(" 2         : 当前电梯状态\n");
    printf(" 3         : 模拟到下一个单位时间\n");
    printf(" 4 N       : 模拟到下N个单位时间(N为数字)\n");
    printf(" 5 I O P A : 添加用户(I为呼叫楼层, O为目的楼层, P为容忍单位时间, A为进出电梯单位时间)\n");
    printf(" 6         : 设置变量\n");
    printf(" 7         : 重置\n");
    printf(" 8         : 退出\n");
    printf(" > ");
}

/**
 * 输出环境变量
 *
 * @return void
 */
void print_env(simulatorStru * simulator) {
    printf(" 当前环境变量\n");
    printf(" [0] 电梯数量: %d\n", (*simulator)->elevator);
    printf(" [1] 最底楼层: %d\n", (*simulator)->min_floor);
    printf(" [2] 最顶楼层: %d\n", (*simulator)->max_floor);
    printf(" [4] 本垒楼层: %d\n", (*simulator)->base_floor);
    printf(" [5] 返回\n");
    printf(" > ");
}

/**
 * 初始化模拟器
 *
 * @param simulator 模拟器
 * @param elevator 电梯数量
 * @param base_floor 本垒层
 * @param min_floor 最底层
 * @param max_floor 最顶层
 * @return void
 */
void init_simulator(simulatorStru * simulator, int elevator, int base_floor, int min_floor, int max_floor) {
    *simulator = (simulatorStru) malloc (sizeof(simStru));
    (*simulator)->customer = 0;
    (*simulator)->elevator = 0;  // 当前楼梯数量初始化为 0, 在添加楼梯函数中会动态修改此值
    (*simulator)->base_floor = base_floor;
    (*simulator)->min_floor = min_floor;
    (*simulator)->max_floor = max_floor;
    (*simulator)->current_time = 0;  // 当前时间初始化为 0
    (*simulator)->max_idle_time = 300;
    int idx;
    for(idx = 0; idx < elevator; idx++) {
        // 添加电梯
        add_elevator(simulator, (*simulator)->base_floor);
    }
}

/**
 * 输出当前单位时间状态
 *
 * @param simulator 模拟器
 * @return void
 */
void print_cur_state(simulatorStru * simulator) {
    int idx;
    printf(" >>>>>>>>>>>>>>>> 当前单位时间: %d\n", (*simulator)->current_time);
    printf(" >>>>>>>>>>>>>>>> 电梯数量: %d\n", (*simulator)->elevator);
    printf(" >>>>>>>>>>>>>>>> 用户数量: %d\n", (*simulator)->customer);
    printf(" >>>>>>>>>>>>>>>> 呼叫信号: ");
    for(idx = 0; idx < (*simulator)->calling_number; idx ++) {
        printf("%d, ", (*simulator)->calling_queue[idx]);
    }
    printf("\n");
    // 输出电梯状态
    printf(" 电梯状态:\n");
    for(idx = 0; idx < (*simulator)->elevator; idx ++) {
        elevatorStru elev;
        elev = (*simulator)->elevator_queue[idx];
        printf(" [%d] 电梯ID:     %d\n", idx, elev->id);
        printf("      电梯位置:    %d\n", elev->current_floor);
        printf("      当前状态:    %s\n", get_elev_state(elev->state));
        printf("      载有用户数量: %d\n", elev->customer);
    }
    // 输出用户状态
    printf(" 用户状态:\n");
    for(idx = 0; idx < (*simulator)->customer; idx ++) {
        customerStru cust;
        cust = (*simulator)->customer_queue[idx];
        printf(" [%d] 用户ID:       %d\n", idx, cust->id);
        printf("      呼叫位置:      %d\n", cust->in_floor);
        printf("      目标位置:      %d\n", cust->out_floor);
        printf("      当前状态:      %s\n", get_cust_state(cust->state));
        printf("      所在电梯ID:    %d\n", cust->elevator_id);
        printf("      当前状态时间:   %d\n", cust->state_time);
    }
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^\n");
}

/**
 * 模拟到下 N 个单位时间
 *
 * @param simulator 模拟器
 * @param time 单位时间
 * @return void
 */
void next_unit(simulatorStru * simulator, int time) {
    int next;

    for(next = 1; next <= time; next ++) {
        //  更新各个电梯状态
        update_elevator(simulator);

        // 更新各个用户状态
        update_customer(simulator);

        (*simulator)->current_time ++;
        // 输出当前单位时间状态
        print_cur_state(simulator);
    }
}

/**
 * 更新各个电梯状态
 *
 * @param simualtor 模拟器
 * @return void
 */
void update_elevator(simulatorStru * simulator) {
    int idx;
    for(idx = 0; idx < (*simulator)->elevator; idx ++) {
        elevatorStru elev;
        elev = (*simulator)->elevator_queue[idx];

        elev->state_time ++;  // 自增电梯状态时间
        // 更新电梯状态
        switch(elev->state) {
        case Idle:
            if(elev->state_time >= (*simulator)->max_idle_time) {
                // 电梯静止时间到达最大空闲时间, 返回本垒层
                go_to(&elev, elev->base_floor);
            } else {
                get_calling(simulator, &elev);
            }
            break;
        case Uping:
            if(elev->state_time == elev->move_time) {
                // 电梯上升一层
                elev->current_floor ++;
                if(elev->current_floor == elev->destination) {
                    // 电梯到达目的楼层
                    elev->state = Opening;  // 变换状态为"正在打开门"
                    elev->state_time = 0;  // 置 0 当前状态时间
                } else {
                    add_up_customer(simulator, &elev, elev->current_floor);
                    if(elev->in_floor_front < elev->in_floor_rear) {
                        elev->state = Opening;
                        elev->state_time = 0;
                    }
                }
            }
            break;
        case Downing:
            if(elev->state_time == elev->move_time) {
                // 电梯下降一层
                elev->current_floor --;
                if(elev->current_floor == elev->destination) {
                    // 电梯到达目的楼层
                    elev->state = Opening;  // 变换状态为"正在打开门"
                    elev->state_time = 0;  // 置 0 当前状态时间
                } else {
                    add_down_customer(simulator, &elev, elev->current_floor);
                    if(elev->in_floor_front < elev->in_floor_rear) {
                        elev->state = Opening;
                        elev->state_time = 0;
                    }
                }
            }
            break;
        case Opening:
            if(elev->state_time == elev->gate_op_time) {
                // 电梯完成开门动作
                elev->state = Opened;  // 变换状态为"已打开"
                elev->state_time = 0;  // 置 0 当前状态时间
            }
            break;
        case Opened:
            // 等待用户进入或出去
            if(out_elevator(&elev) == true) {
                // 电梯内到达目标楼层的用户都出来了
                // 等待用户进入电梯
                in_elevator_queue(simulator, &elev);
                if(in_elevator(&elev) == true) {
                    // 等待用户都进入电梯
                    // 重置电梯参数
                    elev->in_floor_front = 0;
                    elev->in_floor_rear = 0;
                    elev->out_floor_front = 0;
                    elev->out_floor_rear = 0;
                    elev->state = Closing;
                    elev->state_time = 0;
                }
            }
            break;
        case Closing:
            if(elev->state_time == elev->gate_op_time) {
                // 电梯完成关门动作
                // TODO: 设置目的楼层
            }
            break;
        }
    }
}

/**
 * 更新各个用户状态
 *
 * @param simualtor 模拟器
 * @return void
 */
void update_customer(simulatorStru * simulator) {
    int idx;
    for(idx = 0; idx < (*simulator)->customer; idx ++) {
        customerStru cust;
        cust = (*simulator)->customer_queue[idx];

        switch(cust->state) {
        case Waiting:
            cust->state_time ++;
            if(cust->state_time >= cust->patient_time) {
                // 用户等待时间达到最大容忍时间, 离开
                cust->state = Leaving;
            }
            break;
        case Accessing:
            // 用户正在进入电梯
            break;
        case Ongoing:
            // 用户正在电梯内
            break;
        case Leaving:
            // 用户离开
            // TODO: 删除用户
            break;
        }
    }
}

/**
 * 添加用户
 *
 * @param simulator 模拟器
 * @param in_floor 用户源楼层
 * @param out_floor 用户目标楼层
 * @param patient 用户容忍时间
 * @param access 用户进或出电梯耗费的时间
 * @return void
 */
void add_customer(simulatorStru * simulator, int in_floor, int out_floor, int patient, int access) {
    customerStru customer;
    customer = (customerStru) malloc (sizeof(custStru));
    customer->id = get_customer_id();
    customer->in_floor = in_floor;
    customer->out_floor = out_floor;
    customer->patient_time = patient;
    customer->state_time = 0;
    customer->access_time = access;
    customer->state = Waiting;
    customer->elevator_id = 0;
    (*simulator)->customer_queue[(*simulator)->customer ++] = customer;
    set_new_calling(simulator, customer->in_floor);
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
    elevator->base_floor = base;
    elevator->current_floor = base;
    elevator->state = Idle;
    elevator->gate_op_time = 20;
    elevator->move_time = 30;
    elevator->state_time = 0;
    elevator->destination = 0;
    elevator->in_floor_front = 0;
    elevator->in_floor_rear = 0;
    elevator->out_floor_front = 0;
    elevator->out_floor_rear = 0;
    (*simulator)->elevator_queue[(*simulator)->elevator++] = elevator;
}

#endif /* SIMULATOR_H */
