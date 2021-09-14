# tc practice
show the [code](tc-drop-tcp.c)

## 编译
```
clang -O2 -Wall -target bpf -I /usr/include/x86_64-linux-gnu/ -c tc-drop-tcp.c -o tc-drop-tcp.o
```

## 容器访问外部网络
使用netns命令，修改dns域名服务器，详见项目[Docker-Practice](https://github.com/lbbxsxlz/Docker-Practice/tree/master/DemoShow/netns.md)
```
sudo ip netns exec test-show curl www.baidu.com 
```

CTYPE html>
<!--STATUS OK--><html> <head><meta http-equiv=content-type content=text/html;charset=utf-8><meta http-equiv=X-UA-Compatible content=IE=Edge><meta content=always name=referrer><link rel=stylesheet type=text/css href=http://s1.bdstatic.com/r/www/cache/bdorz/baidu.min.css><title>百度一下，你就知道</title></head> <body link=#0000cc> <div id=wrapper> <div id=head> <div class=head_wrapper> <div class=s_form> <div class=s_form_wrapper> <div id=lg> <img hidefocus=true src=//www.baidu.com/img/bd_logo1.png width=270 height=129> </div> <form id=form name=f action=//www.baidu.com/s class=fm> <input type=hidden name=bdorz_come value=1> <input type=hidden name=ie value=utf-8> <input type=hidden name=f value=8> <input type=hidden name=rsv_bp value=1> <input type=hidden name=rsv_idx value=1> <input type=hidden name=tn value=baidu><span class="bg s_ipt_wr"><input id=kw name=wd class=s_ipt value maxlength=255 autocomplete=off autofocus></span><span class="bg s_btn_wr"><input type=submit id=su value=百度一下 class="bg s_btn"></span> </form> </div> </div> <div id=u1> <a href=http://news.baidu.com name=tj_trnews class=mnav>新闻</a> <a href=http://www.hao123.com name=tj_trhao123 class=mnav>hao123</a> <a href=http://map.baidu.com name=tj_trmap class=mnav>地图</a> <a href=http://v.baidu.com name=tj_trvideo class=mnav>视频</a> <a href=http://tieba.baidu.com name=tj_trtieba class=mnav>贴吧</a> <noscript> <a href=http://www.baidu.com/bdorz/login.gif?login&amp;tpl=mn&amp;u=http%3A%2F%2Fwww.baidu.com%2f%3fbdorz_come%3d1 name=tj_login class=lb>登录</a> </noscript> <script>document.write('<a href="http://www.baidu.com/bdorz/login.gif?login&tpl=mn&u='+ encodeURIComponent(window.location.href+ (window.location.search === "" ? "?" : "&")+ "bdorz_come=1")+ '" name="tj_login" class="lb">登录</a>');</script> <a href=//www.baidu.com/more/ name=tj_briicon class=bri style="display: block;">更多产品</a> </div> </div> </div> <div id=ftCon> <div id=ftConw> <p id=lh> <a href=http://home.baidu.com>关于百度</a> <a href=http://ir.baidu.com>About Baidu</a> </p> <p id=cp>&copy;2017&nbsp;Baidu&nbsp;<a href=http://www.baidu.com/duty/>使用百度前必读</a>&nbsp; <a href=http://jianyi.baidu.com/ class=cp-feedback>意见反馈</a>&nbsp;京ICP证030173号&nbsp; <img src=//www.baidu.com/img/gs.gif> </p> </div> </div> </div> </body> </html>


## 添加队列与过滤器
查看队列
```
tc qdisc show dev vethd26706b
```

	qdisc noqueue 0: root refcnt 2 

添加队列，自带的tc命令添加会失败，用了自己编译的高版本
```
sudo ~/workspace/iproute2-5.11.0/tc/tc qdisc add dev vethd26706b clsact
tc qdisc show dev vethd26706b
```

	qdisc noqueue 0: root refcnt 2 
	qdisc clsact ffff: parent ffff:fff1 

查看过滤器出口规则
```
sudo ~/workspace/iproute2-5.11.0/tc/tc filter show dev vethd26706b egress
```

添加过滤器
```
sudo ~/workspace/iproute2-5.11.0/tc/tc filter add dev vethd26706b egress bpf da obj tc-drop-tcp.o sec tc-tcp verbose
```

	Continuing without mounted eBPF fs. Too old kernel?

	Prog section 'tc-tcp' loaded (5)!
	 - Type:         3
	 - Instructions: 19 (0 over limit)
	 - License:      GPL

	Verifier analysis:

	0: (b7) r0 = 2
	1: (61) r2 = *(u32 *)(r1 +80)
	2: (61) r1 = *(u32 *)(r1 +76)
	3: (bf) r3 = r1
	4: (07) r3 += 14
	5: (2d) if r3 > r2 goto pc+12
	 R0=inv2 R1=pkt(id=0,off=0,r=14,imm=0) R2=pkt_end(id=0,off=0,imm=0) R3=pkt(id=0,off=14,r=14,imm=0) R10=fp0
	6: (bf) r3 = r1
	7: (07) r3 += 34
	8: (2d) if r3 > r2 goto pc+8
	 R0=inv2 R1=pkt(id=0,off=0,r=34,imm=0) R2=pkt_end(id=0,off=0,imm=0) R3=pkt(id=0,off=34,r=34,imm=0) R10=fp0
	9: (71) r2 = *(u8 *)(r1 +13)
	10: (67) r2 <<= 8
	11: (71) r3 = *(u8 *)(r1 +12)
	12: (4f) r2 |= r3
	13: (57) r2 &= 65535
	14: (55) if r2 != 0x8 goto pc+2
	 R0=inv2 R1=pkt(id=0,off=0,r=34,imm=0) R2=inv8 R3=inv(id=0,umax_value=255,var_off=(0x0; 0xff)) R10=fp0
	15: (71) r1 = *(u8 *)(r1 +23)
	16: (15) if r1 == 0x6 goto pc+1
	 R0=inv2 R1=inv(id=0,umax_value=255,var_off=(0x0; 0xff)) R2=inv8 R3=inv(id=0,umax_value=255,var_off=(0x0; 0xff)) R10=fp0
	17: (b7) r0 = 0
	18: (95) exit

	from 16 to 18: R0=inv2 R1=inv6 R2=inv8 R3=inv(id=0,umax_value=255,var_off=(0x0; 0xff)) R10=fp0
	18: (95) exit

	from 14 to 17: safe

	from 8 to 17: safe

	from 5 to 18: safe
	processed 23 insns, stack depth 0

再次查看过滤器出口规则
```
sudo ~/workspace/iproute2-5.11.0/tc/tc filter show dev vethd26706b egress
```

	filter protocol all pref 49152 bpf chain 0 
	filter protocol all pref 49152 bpf chain 0 handle 0x1 tc-drop-tcp.o:[tc-tcp] direct-action not_in_hw id 10 tag e4e8d89f5834ec06 jited 

此时容器访问外部网络，使用以上的ip netns命令，无输出

然后删除规则
```
sudo ~/workspace/iproute2-5.11.0/tc/tc filter del dev vethd26706b egress
```
卡住的curl再次输出信息，查看规则，为空


最后删除添加的队列
```
sudo ~/workspace/iproute2-5.11.0/tc/tc qdisc del dev vethd26706b clsact
```

再次查看规则，输出与最初的一致

