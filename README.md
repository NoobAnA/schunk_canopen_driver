# schunk_lwa4p机械臂在ubuntu18.04驱动安装

## PeakCan接口在ros环境使用schunk_lwa4p机械臂参考文档地址

[schunk_canopen_driver](https://github.com/fzi-forschungszentrum-informatik/schunk_canopen_driver)

## ubuntu14.04下源文件

[schunk_canopen_driver](https://github.com/fzi-forschungszentrum-informatik/schunk_canopen_driver)

[fzi_icl_core](https://github.com/fzi-forschungszentrum-informatik/fzi_icl_core)

[fzi_icl_can](https://github.com/fzi-forschungszentrum-informatik/fzi_icl_can)



## ubuntu18.04下编译error修改

> lwa4p_ws/src/fzi_icl_core/src/icl_core/src/ts/icl_core_thread/ts_PeriodicThread.cpp:185:3: error: ‘BOOST_MESSAGE’ was not declared in this scope

修改对应.cpp文件第185行*BOOST_MESSAGE*为*BOOST_TEST_MESSAGE*

> lwa4p_ws/src/fzi_icl_core/src/icl_core/src/ts/icl_core_config/ts_main.cpp:44:1: error: expected initializer at end of input
>  BOOST_GLOBAL_FIXTURE(GlobalFixture)

修改对应.cpp文件第44行为*BOOST_GLOBAL_FIXTURE(GlobalFixture);*

> lwa4p_ws/src/fzi_icl_core/src/icl_core/src/ts/icl_core_thread/ts_main.cpp:43:1: error: expected initializer at end of input
>  BOOST_GLOBAL_FIXTURE(tGlobalFixture)

修改对应.cpp文件第43行为*BOOST_GLOBAL_FIXTURE(tGlobalFixture);*

> lwa4p_ws/src/schunk_canopen_driver/src/icl_hardware_canopen/src/icl_hardware_canopen/SchunkPowerBallNode.h:57:23: error: ‘constexpr’ needed for in-class initialization of static data member ‘const double icl_hardware::canopen_schunk::SchunkPowerBallNode::RAD_TO_STEPS_FACTOR’ of non-integral type [-fpermissive]

修改对应.h文件第57行为*static constexpr double RAD_TO_STEPS_FACTOR = 57295.7795131;*

> CMakeFiles/icl_hardware_canopen_test.dir/test_emcy.cpp.o: In function `main':
> /usr/include/boost/test/impl/unit_test_main.ipp:274: multiple definition of `main'

头文件重复定义，注释*lwa4p_ws/src/schunk_canopen_driver/src/icl_hardware_canopen/src/icl_hardware_canopen/test/testing_main.cpp*第25行

> CMakeFiles/icl_hardware_canopen_test.dir/test_emcy.cpp.o: In function `main':
> /usr/include/boost/test/impl/unit_test_main.ipp:283: undefined reference to `init_unit_test_suite(int, char**)'

在*lwa4p_ws/src/schunk_canopen_driver/src/icl_hardware_canopen/src/icl_hardware_canopen/test/test_emcy.cpp*开头添加

```c++
#define BOOST_TEST_MAIN
```



## 使用PCAN-USB adapter 需要在工作空间安装*libpcan*

[libpcan](http://wiki.ros.org/libpcan)

To activate the driver you have to restart the system or if you want to avoid that manual load the driver with:


  ```bash
  sudo modprobe pcan
  ```


  After restart the driver will be activated automatically.

  You can verify the result with the command:


  ```bash
  ls -l /dev/pcan*
  ```







