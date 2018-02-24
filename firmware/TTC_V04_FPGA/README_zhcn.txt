9.23
接收测试，存在丢包现象，暂时发现是4463配置不对造成的数据错误丢包。
Glue_Logic将CPU读数据部分将大端32位改为小端32位，即是
wire [31:0]liyewei;
assign liyewei={Data_In[7:0],Data_In[15:8],Data_In[23:16],Data_In[31:24]} ;

9.29
补全了Glue_Logic,逻辑部分添加了包计数等功能，Glud_Logic的地址改为使用reg， CPU去读取，在板上测试成功。
 
9.30 
修改TC包计数的位置，测试成功，Glud_Logic的地址改回使用parameter

10.6
CLTU增加正在接收的标志位Flag_RX_ing，
检测到16'heb90将Flag_RX_ing置位，检测到错误或者结尾序列清零
已仿真，未上板测试

----------
更新逻辑部分引脚定义
----------


10.19
更改tx配置文件，由黎明生成

10.27
上传IP包 从32块缩减为4块  。



11.1
终版本。上行2k，下行40k
