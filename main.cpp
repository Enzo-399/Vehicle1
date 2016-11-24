#include <iostream>
#include <time.h>
#include <malloc.h>
#include <cstring>

using namespace std;

typedef int ElemType;

const ElemType parkSize = 2;          //ͣ����ͣ��������
const int vehicleNumber = 10;         //���ƺ��ַ�����
const double priceHour = 10;          //ÿСʱͣ�����շѼ۸�

struct Vehicle{                       //�����Ľṹ��
    char vhlNumber[vehicleNumber];    //���ƺ�����
    time_t arrivalTime;               //���������ʱ��
    time_t departTime;                //�����뿪��ʱ��
};

struct ParkStack{
    //ͣ��������
    Vehicle parkVehicle[parkSize];    //ͣ��λ����
    ElemType top;
};

struct TempStack{
    //��ʱջ
    Vehicle tempVehicle[parkSize];
    ElemType top;
};

struct WaitNode{
    //�ȴ����нڵ�����
    Vehicle waitVehicle;
    WaitNode *next;
};

struct WaitQueue{
    //�ȴ���������
    WaitNode *front;
    WaitNode *rear;
};

void InitPackStack(ParkStack &parkStack){
    //��ʼ��ͣ����
    parkStack.top = -1;
}

void InitTempStack(TempStack &tempStack){
    //��ʼ����ʱջ
    tempStack.top = -1;
}

void InitWaitQueue(WaitQueue &waitQueue){
    //��ʼ���ȴ�����
    waitQueue.front = (WaitNode *) malloc(sizeof(WaitNode));
    waitQueue.front->next = NULL;
    waitQueue.rear = waitQueue.front;
}

ElemType PushPark(ParkStack &parkStack,Vehicle &vehicle){
//��������ͣ����
    if (parkStack.top == parkSize - 1) {
        //��ͣ������������0
        return 0;
    } else {
        parkStack.top++;
        parkStack.parkVehicle[parkStack.top] = vehicle;
        time_t t;

        vehicle.arrivalTime = time(&t);       //��¼�½���ͣ������ʱ��
    }
    return parkStack.top + 1;                 //���س������ڵĳ�λ
}

ElemType IsParkStackEmpty(ParkStack &parkStack){
    //�ж�ͣ�����Ƿ�Ϊ��
    if (parkStack.top == -1) {
        return 1;
    } else {
        return 0;
    }
}

ElemType IsQueueEmpty(WaitQueue &waitQueue){
    //�жϵȴ��Ķ����Ƿ�Ϊ��
    if (waitQueue.front == waitQueue.rear) {
        return 1;
    } else {
        return 0;
    }
}

void EnQueue(Vehicle &vehicle, WaitQueue &waitQueue){
    //�����ڱ�����Ŷ�
    WaitNode *temp;
    temp = (WaitNode *) malloc(sizeof(WaitNode));
    temp->next = NULL;
    if (vehicle.vhlNumber) {
        strcpy(temp->waitVehicle.vhlNumber, vehicle.vhlNumber);         //�����ĳ��ƺŸ�ֵ���ȴ������ĳ��ƺ�
        waitQueue.rear->next = temp;
        waitQueue.rear = temp;
    }
}

void Arrival(ParkStack &parkStack,Vehicle &vehicle,WaitQueue &waitQueue){
    //��������
    cout << "�����뵽�������ĳ��ƺţ�";
    cin >> vehicle.vhlNumber;
    ElemType i = PushPark(parkStack, vehicle);
    if (i) {
        //�����Ѿ��ɹ�����ͣ����������������Ϣ
        cout << "�����ѽ���ͣ������λ�ڵڡ�" << i << "������λ" << endl;
        cout << "��������ͣ������ʱ���ǣ�" << ctime(&(vehicle.arrivalTime));
    } else {
        //ͣ����������֪ͨ�ͻ�ȥ����Ŷ�
        cout << "ͣ���������޿�λ����ȥ����Ŷӣ�" << endl;
        EnQueue(vehicle, waitQueue);    //����
        cout << "���ĳ��ѳɹ��ڱ���Ⱥ�" << endl;//����֪������Ա���ͻ��ѳɹ��Ŷ�
    }
}

ElemType LeavePark(ElemType &location,ParkStack &parkStack,WaitQueue &waitQueue){
    //�����뿪ͣ����
    TempStack tempStack;
    InitTempStack(tempStack);           //������ʱջ������ʼ��

    if (parkStack.top == -1) {
        //��ͣ�����գ��򷵻�
        cout << "��ʱͣ�������޳�����" << endl;
        return 0;
    } else if (parkStack.top + 1 == location) {
        //���뿪�ĳ���λ��ջ��
        cout << "���ƺ�Ϊ��" << parkStack.parkVehicle[parkStack.top].vhlNumber << "�ĳ����Ѿ��뿪ͣ����" << endl;
        time_t t;

        parkStack.parkVehicle[parkStack.top].departTime = time(&t);
        cout << "�뿪ʱ���ǣ�" << ctime(&(parkStack.parkVehicle[parkStack.top].departTime));
        double timediff = difftime(parkStack.parkVehicle[parkStack.top].departTime,
                                   parkStack.parkVehicle[parkStack.top].arrivalTime) / 3600;
        double money = timediff * priceHour;

        cout << "�軨�ѣ�" << money << "Ԫ" << endl;
        parkStack.top--;
    } else {    //�뿪����������ջ��
        while (1) {
            tempStack.tempVehicle[++tempStack.top] = parkStack.parkVehicle[parkStack.top--];              //����ʱջ����
            if (parkStack.top == location - 1) {
                break;
            }
        }
        cout << "���ƺ�Ϊ��" << parkStack.parkVehicle[parkStack.top].vhlNumber << "�ĳ����Ѿ��뿪ͣ����" << endl;
        time_t t;

        parkStack.parkVehicle[parkStack.top].departTime = time(&t);
        cout << "�뿪ʱ��Ϊ��" << ctime(&(parkStack.parkVehicle[parkStack.top].departTime));
        double timediff = difftime(parkStack.parkVehicle[parkStack.top].departTime,
                                   parkStack.parkVehicle[parkStack.top].arrivalTime) / 3600;
        double money = timediff * priceHour;
        cout.precision(2);
        cout << "�轻�ѣ�" << money << "Ԫ" << endl;

        while (tempStack.top != -1) {
            parkStack.parkVehicle[parkStack.top++] = tempStack.tempVehicle[tempStack.top--];
        }
        parkStack.top--;
    }
    return 1;
}

void DeleteQueue(ParkStack &parkStack, WaitQueue &waitQueue) {
    //�����ӱ������ͣ����
    if (waitQueue.front->next->next == NULL) {
        //������ֻ��һ����
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
    //�����뿪ͣ����
    ElemType location;
    cout << "ͣ�����й���" << parkStack.top + 1 << "����" << endl;
    if (IsParkStackEmpty(parkStack)) {
        cout << "��ʱͣ�������޳������" << endl;
    } else {
        cout << "�����뽫Ҫ�뿪ͣ�����ĳ������ڵĳ�λ��";
        cin >> location;
        if (location < 0 || location > parkStack.top + 1) {
            cout << "������ĳ�λ����������ѡ��" << endl;
            return;
        }
    }
    ElemType j = LeavePark(location, parkStack, waitQueue); //���ó�ջ����
    if (j) {
        if (!IsQueueEmpty(waitQueue)) {
            DeleteQueue(parkStack, waitQueue);
            cout << endl;

            cout << "���ƺ�Ϊ��" << parkStack.parkVehicle[parkStack.top].vhlNumber<<"�ĳ����ѽ���ͣ����"<<endl;
            cout << endl;
        } else {
            cout << "��ʱ������޳����Ŷӣ�" << endl;
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
    //ѡ����ƽ���
        cout << "��ѡ��A���������D�������뿪��E���˳�����" << endl;
        cout << "��ѡ��" << endl;
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
                cout << "���볬����Χ��" << endl;
                break;
        }
    }
}