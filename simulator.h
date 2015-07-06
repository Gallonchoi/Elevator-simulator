#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdio.h>
#include "core.h"

void read_command();
void print_cur_state();
void print_help();
void print_env();
void next_unit(int unit, bool is_jump);
void init_simulator();
void add_customer(simulatorStru * simulator, int in_floor, int out_floor, int patient, int access);
void add_elevator(simulatorStru * simulator, int base);

/**n
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
            break;
        case 4:
            // 模拟到下N个单位时间
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
    (*simulator)->cust_patient = 100;
    (*simulator)->cust_access_time = 25;
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
    printf(" >>>>>>>>>>>>>>>> 当前单位时间: %d\n", (*simulator)->current_time);
    printf(" >>>>>>>>>>>>>>>> 电梯数量: %d\n", (*simulator)->elevator);
    printf(" >>>>>>>>>>>>>>>> 用户数量: %d\n", (*simulator)->customer);
    // 输出电梯状态
    printf(" 电梯状态:\n");
    int idx;
    for(idx = 0; idx < (*simulator)->elevator; idx++) {
        elevatorStru elev;
        elev = (*simulator)->elevator_queue[idx];
        printf(" [%d] 电梯ID:     %d\n", idx, elev->id);
        printf("      电梯位置:    %d\n", elev->current_position);
        printf("      当前状态:    %s\n", get_elev_state(elev->state));
        printf("      载有用户数量: %d\n", elev->customer);
    }
    // 输出用户状态
    printf(" 用户状态:\n");
    for(idx = 0; idx < (*simulator)->customer; idx++) {
        customerStru cust;
        cust = (*simulator)->customer_queue[idx];
        printf(" [%d] 用户ID:     %d\n", idx, cust->id);
        printf("      呼叫位置:    %d\n", cust->in_floor);
        printf("      目标位置:    %d\n", cust->out_floor);
        printf("      当前状态:    %s\n", get_cust_state(cust->state));
        printf("      等待时间:    %d\n", cust->waiting_time);
    }
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^\n");
}

void next_unit(int unit, bool is_jump) {}

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
    customer->waiting_time = 0;
    customer->access_time = access;
    customer->state = Waiting;
    (*simulator)->customer_queue[(*simulator)->customer ++] = customer;
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
    (*simulator)->elevator_queue[(*simulator)->elevator++] = elevator;
}

#endif /* SIMULATOR_H */
