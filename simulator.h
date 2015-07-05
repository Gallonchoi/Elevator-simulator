#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdio.h>
#include "core.h"

simulatorStru simulator = NULL;  // 全局模拟器
int elevator_number = 3;  // 默认电梯数量
int base_floor = 1;  // 默认本垒层
int min_floor = -1;  // 默认最底层
int max_floor = 10;  // 默认最顶层
/* double unit_of_time = 0.02;  // 默认单位时间 */

void read_command();
void print_cur_state();
void print_help();
void print_env();
void next_unit(int unit, bool is_jump);
void init_simulator();
/* void add_elev(int base); */
/* void add_cust(int source, int dest, int patient); */

/**
 * 读取并解析命令
 *
 * @return void
 */
void read_command() {
    int command_code = 1;
    bool is_exit = false;  // 退出标志
    while(is_exit == false) {
        switch(command_code) {
        case 1:
            // 输出帮助
            print_help();
            break;
        case 2:
            // 输出当前状态
            break;
        case 3:
            // 模拟到下一个单位时间
            break;
        case 4:
            // 模拟到下N个单位时间
            break;
        case 5:
            // 添加用户
            break;
        case 6:
            // 设置环境变量
            print_env();
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
                    elevator_number = temp;
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
                    min_floor = temp;
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
                    max_floor = temp;
                }
                break;
            case 3:
                // 设置本垒楼层
                printf("本垒楼层(%d ~ %d) = ", min_floor, max_floor);
                scanf("%d", &temp);
                if(temp < min_floor || temp > max_floor || temp == 0) {
                    printf("=========================\n");
                    printf("参数错误, 本垒楼层不能小于最底楼层或大于最顶楼层或等于0\n");
                    printf("=========================\n");
                } else {
                    base_floor = temp;
                }
                break;
            default:
                printf("=========================\n");
                printf("未知命令\n");
                printf("=========================\n");
                break;
            }
            print_env();
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
    printf(" 1     : 输出帮助\n");
    printf(" 2     : 当前电梯状态\n");
    printf(" 3     : 模拟到下一个单位时间\n");
    printf(" 4 N   : 模拟到下N个单位时间(N为数字)\n");
    printf(" 5 S D : 添加用户(S为初始位置, D为目的位置)\n");
    printf(" 6     : 设置变量\n");
    printf(" 7     : 重置\n");
    printf(" 8     : 退出\n");
    printf(" > ");
}

/**
 * 输出环境变量
 *
 * @return void
 */
void print_env() {
    printf(" 当前环境变量\n");
    printf(" [0] 电梯数量: %d\n", elevator_number);
    printf(" [1] 最底楼层: %d\n", min_floor);
    printf(" [2] 最顶楼层: %d\n", max_floor);
    printf(" [4] 本垒楼层: %d\n", base_floor);
    printf(" [5] 返回\n");
    printf(" > ");
}

/**
 * 初始化模拟器
 *
 * @return void
 */
void init_simulator() {
    free(simulator);
    init(&simulator);
    int idx;
    for(idx = 0; idx < elevator_number; idx++) {
        // 添加电梯
        add_elevator(&simulator, base_floor);
    }
}

void print_cur_state() {}
void next_unit(int unit, bool is_jump) {}

#endif /* SIMULATOR_H */
