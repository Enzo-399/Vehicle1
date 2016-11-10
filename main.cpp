#include <iostream>
#include <time.h>
#include <malloc.h>
#include <cstring>

using namespace std;

typedef int ElemType;

const ElemType parkSize = 2;          //停车场停车的容量
const int vehicleNumber = 10;         //车牌号字符数量
const double priceHour = 10;          //每小时停车的收费价格

struct Vehicle{                       //车辆的结构体
    char vhlNumber[vehicleNumber];    //车牌号数组
    time_t arrivalTime;               //汽车到达的时间
    time_t departTime;                //汽车离开的时间
};

struct ParkStack{
    //停车场类型
    Vehicle parkVehicle[parkSize];
    ElemType top;
};

struct TempStack{
    //临时栈
    Vehicle tempVehicle[parkSize];
    ElemType top;
};

struct WaitNode{
    //等待队列节点类型
    Vehicle waitVehicle;
    WaitNode *next;
};

struct WaitQueue{
    //等待队列类型
    WaitNode *front;
    WaitNode *rear;
};

void InitPackStack(ParkStack &parkStack){
    //初始化停车场
    parkStack.top = -1;
}

void InitTempStack(TempStack &tempStack){
    //初始化临时栈
    tempStack.top = -1;
}

void InitWaitQueue(WaitQueue &waitQueue){
    //初始化队列
    waitQueue.front = (WaitNode *) malloc(sizeof(WaitNode));
    waitQueue.front->next = NULL;
    waitQueue.rear = waitQueue.front;
}

ElemType PushPark(ParkStack &parkStack,Vehicle &vehicle){//车辆进入停车场
    if (parkStack.top == parkSize - 1) {
        //若停车场满，返回0
        return 0;
    } else {
        parkStack.top++;
        parkStack.parkVehicle[parkStack.top] = vehicle;
        time_t t;

        vehicle.arrivalTime = time(&t);       //记录下进入停车场的时间
    }
    return parkStack.top + 1;                 //返回车辆所在的车位
}

ElemType IsParkStackEmpty(ParkStack &parkStack){
    //判断停车场是否为空
    if (parkStack.top == -1) {
        return 1;
    } else {
        return 0;
    }
}

ElemType IsQueueEmpty(WaitQueue &waitQueue){
    //判断等待的队列是否为空
    if (waitQueue.front == waitQueue.rear) {
        return 1;
    } else {
        return 0;
    }
}

void EnQueue(Vehicle &vehicle, WaitQueue &waitQueue){
    //车辆在便道上排队
    WaitNode *temp;
    temp = (WaitNode *) malloc(sizeof(WaitNode));
    temp->next = NULL;
    if (vehicle.vhlNumber) {
        strcpy(temp->waitVehicle.vhlNumber, vehicle.vhlNumber);
        waitQueue.rear->next = temp;
        waitQueue.rear = temp;
    }
}

void Arrival(ParkStack &parkStack,Vehicle &vehicle,WaitQueue &waitQueue){
    //车辆到达
    cout << "请输入到达汽车的车牌号：";
    cin >> vehicle.vhlNumber;
    ElemType i = PushPark(parkStack, vehicle);
    if (i) {
        //车辆已经成功进入停车场，则输出相关信息
        cout << "车辆已进入停车场，位于第“" << i << "个车位" << endl;
        cout << "车辆进入停车场的时间是：" << ctime(&(vehicle.arrivalTime));
    } else {
        //停车场已满，通知客户去便道排队
        cout << "停车场内已无空位，请去便道排队！" << endl;
        EnQueue(vehicle, waitQueue);    //进队
        cout << "您的车已成功在便道等候！" << endl;//并告知管理人员，客户已成功排队
    }
}

ElemType LeavePark(ElemType &location,ParkStack &parkStack,WaitQueue &waitQueue){
    //车辆离开停车场
    TempStack tempStack;
    InitTempStack(tempStack);           //建立临时栈，并初始化

    if (parkStack.top == -1) {
        //若停车场空，则返回
        cout << "此时停车场内无车辆！" << endl;
        return 0;
    } else if (parkStack.top + 1 == location) {
        //若离开的车辆位于栈顶
        cout << "车牌号为：" << parkStack.parkVehicle[parkStack.top].vhlNumber << "的车辆已经离开停车场" << endl;
        time_t t;

        parkStack.parkVehicle[parkStack.top].departTime = time(&t);
        cout << "离开时间是：" << ctime(&(parkStack.parkVehicle[parkStack.top].departTime));
        double timediff = difftime(parkStack.parkVehicle[parkStack.top].departTime,
                                   parkStack.parkVehicle[parkStack.top].arrivalTime) / 3600;
        double money = timediff * priceHour;

        cout << "需花费：" << money << "元" << endl;
        parkStack.top--;
    } else {
        while (1) {
            tempStack.tempVehicle[++tempStack.top] = parkStack.parkVehicle[parkStack.top--];              //与临时栈操作
            if (parkStack.top == location - 1) {
                break;
            }
        }
        cout << "车牌号为：" << parkStack.parkVehicle[parkStack.top].vhlNumber << "的车辆已经离开停车场" << endl;
        time_t t;

        parkStack.parkVehicle[parkStack.top].departTime = time(&t);
        cout << "离开时间为：" << ctime(&(parkStack.parkVehicle[parkStack.top].departTime));
        double timediff = difftime(parkStack.parkVehicle[parkStack.top].departTime,
                                   parkStack.parkVehicle[parkStack.top].arrivalTime) / 3600;
        double money = timediff * priceHour;
        cout.precision(2);
        cout << "需交费：" << money << "元" << endl;

        while (tempStack.top != -1) {
            parkStack.parkVehicle[parkStack.top++] = tempStack.tempVehicle[tempStack.top--];
        }
        parkStack.top--;
    }
    return 1;
}

void DeleteQueue(ParkStack &parkStack, WaitQueue &waitQueue) {
    //车辆从便道进入停车场
    if (waitQueue.front->next->next == NULL) {
        //若队中只有一辆车
        parkStack.parkVehicle[++parkStack.top] = waitQueue.front->next->waitVehicle;
        free(waitQueue.front->next);
        waitQueue.front->next = NULL;
        waitQueue.rear = waitQueue.front;
    } else {
        WaitNode *temp;
        temp = waitQueue.front->next;

        parkStack.parkVehicle[++parkStack.top] = temp->waitVehicle;
        waitQueue.front->next = temp->next;
        free(temp);
    }
}

void DepartPark(ParkStack &parkStack,WaitQueue &waitQueue) {
    //车辆离开停车场
    ElemType location;
    cout << "停车场中共有" << parkStack.top + 1 << "辆车" << endl;
    if (IsParkStackEmpty(parkStack)) {
        cout << "此时停车场中无车辆存放" << endl;
    } else {
        cout << "请输入将要离开停车场的车辆所在的车位：";
        cin >> location;
        if (location < 0 || location > parkStack.top + 1) {
            cout << "您输入的车位错误，请重新选择！" << endl;
            return;
        }
    }
    ElemType j = LeavePark(location, parkStack, waitQueue); //调用出栈函数
    if (j) {
        if (!IsQueueEmpty(waitQueue)) {
            DeleteQueue(parkStack, waitQueue);
            cout << endl;

            cout << "车牌号为：" << parkStack.parkVehicle[parkStack.top].vhlNumber<<"的车辆已进入停车场"<<endl;
            cout << endl;
        } else {
            cout << "此时便道上无车辆排队！" << endl;
        }
    }
}

int main() {
    Vehicle vehicle;
    ParkStack parkStack;
    InitPackStack(parkStack);
    WaitQueue waitQueue;
    InitWaitQueue(waitQueue);
    char choice;
    while (1) {
    //选择控制界面
        cout << "请选择：A：车辆到达、D：车辆离开、E：退出程序" << endl;
        cout << "请选择：" << endl;
        cin >> choice;
        if (choice == 'E') {
            break;
        }
        switch (choice) {
            case 'A':
                Arrival(parkStack, vehicle, waitQueue);
                break;
            case 'D':
                DepartPark(parkStack, waitQueue);
                break;
            default:
                cout << "输入超出范围！" << endl;
                break;
        }
    }
}