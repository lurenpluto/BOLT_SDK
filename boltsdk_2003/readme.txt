安装使用方法:
运行setup.js


目录及文件介绍:
bin            存放生成文件目录
dev\dev     存放开发版dll及lib目录
dev\pr        存放正式版dll及lib目录
include        头文件目录
samples        例子目录
samples\HellBolt 配合HelloBolt系列教程的例子
samples\BoltFox  用Bolt实现浏览器的完整产品开发例子，配套文章在编写中
samples\MineSweeper 用Bolt实现的扫雷的例子，在Windows XP再带的扫雷上做了一些效果
samples\Wizard   各种功能，控件的Demo，代码库。配套文章在编写中
 
例子的使用方法 找到例子里的.sln文件，编译运行,相关xar的编译在setup.js里已经做了

tools        开发相关工具包目录
convert_to_dev.bat    切换到开发板
convert_to_pr.bat    切换到正式版
copydevto.js        拷贝开发版到指定目录
copyprto.js            拷贝正式版到指定目录
setup.js            安装脚本
