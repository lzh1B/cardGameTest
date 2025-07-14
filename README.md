
## 环境要求
- **Cocos2dx 版本**: 3.17
- **Python 版本**: 2.7.x
- **Visual Studio 2022** 必须包含以下组件：
  - `使用C++的桌面开发` 工作负载
  - 在"单个组件"中勾选：
    - MSVC v140 - VS 2015 C++ 生成工具
    - MSVC v141 - VS 2017 C++ 生成工具
    - MSVC v142 - VS 2019 C++ 生成工具
    - Windows 10 SDK (建议同时安装最新版和10.0.17763.0版本)
      
## 注意事项
    -环境准备完成后，创建新的cocos2dx工程，将Classes目录和Resources内的文件放入你的项目对于的目录中
    -在同级目录win32的vs项目工程中即可运行，注意Classes/configs中保存的levelConfig.json配置文件目前只是存放，真正加载的是在Resources内的它的副本

