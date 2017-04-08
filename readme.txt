1, 生成对应平台的Makefile:
   Ubuntu x86_64: ./genMakefiles linux-g; make;
   hisiv100nptl: ./genMakefiles armlinux; make;

2, 在Ubuntu启动模拟设备: ./sldevice sid pwd mydev.config
