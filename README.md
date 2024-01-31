51单片机的应用相关。

采用Proteus8.5设计原理图，keil5开发源程序。

1. Answering，8路抢答器设计

该应用利用中断系统实现8路抢答器的设计。具体实现的要求：

```
1） 按下”开始“按键后才开始抢答，且抢答允许指示灯亮；
2）按下“开始”键后如果有人抢答，则抢答指示灯灭，7段数码管显示抢答者编号。
```

电路图连接如下：

	P3.0端口连接“开始”按键；
	P3.6端口连接一个LED，指示抢答允许；
	P0口接7段数码管，显示抢答者编号；
	P2口接8个独立式按键，依次编号为1~8；
	P1口接抢答者指示灯，当P2口的某一个独立按键按下后，对应的抢答者指示灯亮；