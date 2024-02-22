#include <string>
#include <iostream>
#include <serial/serial.h>
#include <chrono>
#include <cmath>
#include <thread>
#define BAUDRATE 921600
class SerialDriver
{
public:
    // 1 创建串口对象
    serial::Serial robot_serial;
    SerialDriver(const char *port_name) : _port_name(port_name)
    {
        
        // 2.串口初始化
        try
        {
            robot_serial.setPort(port_name);
            robot_serial.setBaudrate(BAUDRATE);
            serial::Timeout timeout = serial::Timeout::simpleTimeout(500);
            robot_serial.setTimeout(timeout);
            robot_serial.open();
            std::printf("Open port success! %s\n", port_name); 
        }
        catch (const serial::IOException &e)
        {
            std::printf("Unable to open port %s\n", port_name); 
            return;
        }
                // Clear the buffer
        robot_serial.flush();

        // IMU驱动线程
        // serial_thread_ = std::thread(&SerialDriver::serialThread, this);
    }
    void serial_write(uint8_t data[], size_t size)
    {
        robot_serial.write(data, size);
    }
    void USR_Packet_Send(uint8_t Master_Id,uint8_t Slave_Id,uint8_t MSG_Id,uint8_t Data_Length,uint8_t Data[])
    {
        uint8_t  Usr_send_buf[300];
        uint16_t cnt=6;
        uint8_t sc=0;
        uint8_t ac=0;
        
        Usr_send_buf[0]=0x33;
        Usr_send_buf[1]=0x33;
        Usr_send_buf[2]=Master_Id;       //Master id
        Usr_send_buf[3]=Slave_Id;   //Slave id
        Usr_send_buf[4]=MSG_Id;         //MSG_ID
        Usr_send_buf[5]=Data_Length;    //data_length

        for(uint8_t i=0;i<Data_Length;i++)
        {
            Usr_send_buf[cnt++]=Data[i];    //data_length
        }
        for(uint16_t i=0;i<Data_Length+6;i++)
        {
            sc+=Usr_send_buf[i];
            ac+=sc;
        }
        Usr_send_buf[cnt++]=sc;
        Usr_send_buf[cnt++]=ac;
        
        serial_write(Usr_send_buf,cnt);

    }

    int float_to_data(float val, uint8_t *data)
    {
        data[0] = *(((uint8_t*)(&val)) + 0);
        data[1] = *(((uint8_t*)(&val)) + 1);
        data[2] = *(((uint8_t*)(&val)) + 2);
        data[3] = *(((uint8_t*)(&val)) + 3);
        return 4;
    }
    ~SerialDriver()
    {
        robot_serial.close();
        serial_thread_.join();
    }
private:
    std::string _port_name;
    std::thread serial_thread_;

    // void serialThread(void)
    // {
    //     printf("Serial thread start!\n");
    //     while (1)
    //     {
    //         if(robot_serial.available())
    //         {
    //             uint8_t data;
    //             robot_serial.read(&data, 1);
    //             // std::cout << static_cast<char>(data);
    //             // printf("%c", data);
    //             // std::cout << data;
    //         }

    //         // std::cout << data;
    //     }
    //     printf("Serial thread end!\n");
    // }


};