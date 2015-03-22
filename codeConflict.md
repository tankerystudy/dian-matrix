# Introduction #

SVN update以后可能会出现冲突，这时候，希望能重视


# Details #

  * SVN update以后可能会出现冲突，这时候，希望能重视，**解决冲突**以后再提交，实在不能解决，也要在注释中说明
  * 一定要保证提交的是能够编译通过的版本！

下面这段代码是冲突的地方，貌似是leeht提交的：

```
    SerialInit(&g_SerialArray[0][0],LINE_LEN*DATA_LEN,1);

    for (y=0; y<LINE_LEN; y++)
       for (x=0; x<DATA_LEN; x++)
        {
            CY= 0;
<<<<<<< .mine
    		//g_SerialArray[y][x] = ~(y+1);
    		g_SerialArray[y][x] = Character[y * LINE_LEN + x];
            GDI_ByteMapping(&g_SerialArray[y][x]);
=======
            g_SerialArray[y][x] = ~(y+1);
            led_drv_InterfaceMap(&g_SerialArray[y][x]);
>>>>>>> .r59
        }
```

---


我来解释一下：
```
<<<<<<< .mine
    		//g_SerialArray[y][x] = ~(y+1);
    		g_SerialArray[y][x] = Character[y * LINE_LEN + x];
            GDI_ByteMapping(&g_SerialArray[y][x]);
=======
```
上面这段是自己写的部分
下面是之前的版本：
```
=======
            g_SerialArray[y][x] = ~(y+1);
            led_drv_InterfaceMap(&g_SerialArray[y][x]);
>>>>>>> .r59
```

提交之前应该把不需要的删除，如下：
```
    SerialInit(&g_SerialArray[0][0],LINE_LEN*DATA_LEN,1);

    for (y=0; y<LINE_LEN; y++)
       for (x=0; x<DATA_LEN; x++)
        {
            CY= 0;

    		g_SerialArray[y][x] = Character[y * LINE_LEN + x];
            GDI_ByteMapping(&g_SerialArray[y][x]);
        }
```

然后提交。

_注意维护SVN的整洁～～_